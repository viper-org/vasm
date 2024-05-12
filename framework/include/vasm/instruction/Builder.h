// Copyright 2023 solar-mist


#ifndef VASM_INSTRUCTION_BUILDER_H
#define VASM_INSTRUCTION_BUILDER_H 1

#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/SingleOperandInstruction.h"
#include "vasm/instruction/TwoOperandInstruction.h"
#include "vasm/instruction/VariableOperandInstruction.h"

#include "vasm/instruction/Label.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Relative.h"
#include "vasm/instruction/operand/String.h"

#include "vasm/lexer/Token.h"

#include <cassert>
#include <vector>

namespace instruction
{
    struct TokenStream
    {
        std::vector<lexing::Token>* tokens;
        size_t* position;
    };

    template<typename T>
    class Builder
    {
    public:
        Builder() {}

        std::unique_ptr<T> parse(TokenStream tokens)
        {
            mTokens = tokens;
            int lineNumber = current().getSourceLocation().line;
            if constexpr (std::derived_from<T, NoOperandInstruction>)
            {
                return std::make_unique<T>(lineNumber);
            }
            else if constexpr (std::derived_from<T, SingleOperandInstruction>)
            {
                codegen::OperandSize size = codegen::OperandSize::None;
                if (current().getTokenType() == lexing::TokenType::Size)
                {
                    std::string text = consume().getText();
                    if (text == "byte") size = codegen::OperandSize::Byte;
                    if (text == "word") size = codegen::OperandSize::Word;
                    if (text == "long") size = codegen::OperandSize::Long;
                    if (text == "quad") size = codegen::OperandSize::Quad;
                }

                return std::make_unique<T>(parseOperand(), size, lineNumber);
            }
            else if constexpr (std::derived_from<T, TwoOperandInstruction>)
            {
                codegen::OperandSize size = codegen::OperandSize::None;
                if (current().getTokenType() == lexing::TokenType::Size)
                {
                    std::string text = consume().getText();
                    if (text == "byte") size = codegen::OperandSize::Byte;
                    if (text == "word") size = codegen::OperandSize::Word;
                    if (text == "long") size = codegen::OperandSize::Long;
                    if (text == "quad") size = codegen::OperandSize::Quad;
                }

                OperandPtr left = parseOperand();
                // TODO: expectToken(lexing::TokenType::Comma);
                consume();
                OperandPtr right = parseOperand();
                return std::make_unique<T>(std::move(left), std::move(right), size, lineNumber);
            }
            else if constexpr(std::derived_from<T, VariableOperandInstruction>)
            {
                codegen::OperandSize size = codegen::OperandSize::None;
                if (current().getTokenType() == lexing::TokenType::Size)
                {
                    std::string text = consume().getText();
                    if (text == "byte") size = codegen::OperandSize::Byte;
                    if (text == "word") size = codegen::OperandSize::Word;
                    if (text == "long") size = codegen::OperandSize::Long;
                    if (text == "quad") size = codegen::OperandSize::Quad;
                }

                OperandPtr left = parseOperand();
                if (current().getTokenType() == lexing::TokenType::Comma)
                {
                    consume();
                    OperandPtr right = parseOperand();
                    if (current().getTokenType() == lexing::TokenType::Comma)
                    {
                        consume();
                        OperandPtr third = parseOperand();
                        return std::make_unique<T>(std::move(left), std::move(right), std::move(third), size, lineNumber);
                    }
                    return std::make_unique<T>(std::move(left), std::move(right), size, lineNumber);
                }
                return std::make_unique<T>(std::move(left), size, lineNumber);
            }
            else if constexpr (std::is_same_v<Label, T>)
            {
                std::string name = consume().getText();
                // TODO: expectToken(lexing::TokenType::Colon);
                consume();

                return std::make_unique<T>(name, true);
            }
        }

    private:
        lexing::Token& current()
        {
            return mTokens.tokens->at(*mTokens.position);
        }

        lexing::Token& peek(int offset)
        {
            return mTokens.tokens->at(*mTokens.position + offset);
        }

        lexing::Token& consume()
        {
            return mTokens.tokens->at((*mTokens.position)++);
        }


        OperandPtr parseOperand()
        {
            switch (current().getTokenType())
            {
                case lexing::TokenType::Register:
                    return parseRegister();

                case lexing::TokenType::Immediate:
                case lexing::TokenType::Identifier:
                case lexing::TokenType::Dollar:
                    return parseImmediate();

                case lexing::TokenType::String:
                    return parseString();

                case lexing::TokenType::LBracket:
                    if (peek(1).getTokenType() == lexing::TokenType::Rel)
                        return parseRelMemory();
                    return parseMemory();
                    
                default:
                    return nullptr;
            }
        }

        ImmediatePtr parseImmediate()
        {
            if (current().getTokenType() == lexing::TokenType::Identifier)
            {
                std::string text = consume().getText();
                if (current().getTokenType() == lexing::TokenType::Ampersand)
                {
                    consume();
                    return std::make_unique<LabelOperand>(text, consume().getText());
                }
                return std::make_unique<LabelOperand>(text);
            }
            else if (current().getTokenType() == lexing::TokenType::Dollar)
            {
                consume();
                return std::make_unique<LabelOperand>("$");
            }
            return std::make_unique<Immediate>(std::stoull(consume().getText(), 0, 0));
        }

        RegisterPtr parseRegister()
        {
            return Register::Get(consume().getText());
        }

        MemoryPtr parseMemory()
        {
            consume(); // LBracket

            RegisterPtr base = parseRegister();
            RegisterPtr index;

            std::optional<int> displacement;
            std::optional<int> scale;
            while (displacement == std::nullopt)
            {
                if (current().getTokenType() == lexing::TokenType::Plus)
                {
                    consume();

                    if (current().getTokenType() == lexing::TokenType::Immediate)
                    {
                        displacement = std::stoi(consume().getText(), 0, 0);
                    }
                    else
                    {
                        assert(current().getTokenType() == lexing::TokenType::Register);
                        index = parseRegister();
                        if (current().getTokenType() == lexing::TokenType::Star)
                        {
                            consume();
                            assert(current().getTokenType() == lexing::TokenType::Immediate);
                            scale = std::stoi(consume().getText(), 0, 0);
                            assert(scale == 1 || scale == 2 || scale == 4 || scale == 8);
                        }
                    }
                }
                else if (current().getTokenType() == lexing::TokenType::Minus)
                {
                    consume();

                    assert(current().getTokenType() == lexing::TokenType::Immediate); // TODO: Proper expectToken
                    displacement = -std::stoi(consume().getText(), 0, 0);
                }
                else break;
            }

            assert(current().getTokenType() == lexing::TokenType::RBracket); // TODO: Proper expectToken
            consume();

            return std::make_unique<Memory>(std::move(base), displacement, std::move(index), scale);
        }

        RelativePtr parseRelMemory()
        {
            consume(); // LBracket
            consume(); // Rel

            OperandPtr operand = parseImmediate();
            LabelOperand* label = static_cast<LabelOperand*>(operand.release());

            std::optional<int> displacement;

            if (current().getTokenType() == lexing::TokenType::Plus)
            {
                consume();
                displacement = std::stoi(consume().getText(), 0, 0);
            }
            else if (current().getTokenType() == lexing::TokenType::Minus)
            {
                consume();
                displacement = -std::stoi(consume().getText(), 0, 0);
            }

            assert(current().getTokenType() == lexing::TokenType::RBracket);
            consume();

            return std::make_unique<Relative>(LabelOperandPtr(label), displacement);
        }

        StringPtr parseString()
        {
            return std::make_unique<String>(consume().getText());
        }

        TokenStream mTokens;
    };
}

#endif