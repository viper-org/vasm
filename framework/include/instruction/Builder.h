// Copyright 2023 solar-mist

#include "codegen/Opcodes.h"
#ifndef VASM_INSTRUCTION_BUILDER_H
#define VASM_INSTRUCTION_BUILDER_H 1

#include "instruction/NoOperandInstruction.h"
#include "instruction/SingleOperandInstruction.h"
#include "instruction/TwoOperandInstruction.h"

#include "instruction/Label.h"

#include "instruction/operand/Immediate.h"
#include "instruction/operand/Label.h"
#include "instruction/operand/Register.h"
#include "instruction/operand/Memory.h"
#include "instruction/operand/String.h"

#include "lexer/Token.h"

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
            if constexpr (std::derived_from<T, NoOperandInstruction>)
            {
                return std::make_unique<T>();
            }
            else if constexpr (std::derived_from<T, SingleOperandInstruction>)
            {
                return std::make_unique<T>(parseOperand());
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
                return std::make_unique<T>(std::move(left), std::move(right), size);
            }
            else if constexpr (std::is_same_v<Label, T>)
            {
                std::string name = consume().getText();
                // TODO: expectToken(lexing::TokenType::Comma);
                consume();

                return std::make_unique<T>(name);
            }
        }

    private:
        lexing::Token& current()
        {
            return mTokens.tokens->at(*mTokens.position);
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
                    return parseMemory();
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
            constexpr int REGISTERS_PER_ENCODING = 4;

            unsigned long long index;
            for (index = 0; index < static_cast<unsigned long long>(codegen::Registers.size()); index++)
            {
                if (codegen::Registers[index] == current().getText())
                {
                    break;
                }
            }
            consume();

            return std::make_unique<Register>(index / REGISTERS_PER_ENCODING, static_cast<codegen::OperandSize>(index % REGISTERS_PER_ENCODING));
        }

        MemoryPtr parseMemory()
        {
            consume(); // LBracket

            RegisterPtr reg = parseRegister();

            std::optional<int> displacement;
            if (current().getTokenType() == lexing::TokenType::Plus)
            {
                consume();

                assert(current().getTokenType() == lexing::TokenType::Immediate); // TODO: Proper expectToken
                displacement = std::stoi(consume().getText(), 0, 0);
            }
            else if (current().getTokenType() == lexing::TokenType::Minus)
            {
                consume();

                assert(current().getTokenType() == lexing::TokenType::Immediate); // TODO: Proper expectToken
                displacement = -std::stoi(consume().getText(), 0, 0);
            }

            assert(current().getTokenType() == lexing::TokenType::RBracket); // TODO: Proper expectToken
            consume();

            return std::make_unique<Memory>(std::move(reg), displacement);
        }

        StringPtr parseString()
        {
            return std::make_unique<String>(consume().getText());
        }

        TokenStream mTokens;
    };
}

#endif