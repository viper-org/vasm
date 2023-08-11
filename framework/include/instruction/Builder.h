// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_BUILDER_H
#define VASM_INSTRUCTION_BUILDER_H 1

#include "instruction/NoOperandInstruction.h"
#include "instruction/SingleOperandInstruction.h"
#include "instruction/TwoOperandInstruction.h"

#include "instruction/Label.h"

#include "instruction/operand/Immediate.h"
#include "instruction/operand/Label.h"
#include "instruction/operand/Register.h"
#include "instruction/operand/String.h"

#include "lexer/Token.h"

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
                OperandPtr left = parseOperand();
                // TODO: expectToken(lexing::TokenType::Comma);
                consume();
                OperandPtr right = parseOperand();
                return std::make_unique<T>(std::move(left), std::move(right));
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
            switch (mTokens.tokens->at(*mTokens.position).getTokenType())
            {
                case lexing::TokenType::Register:
                    return parseRegister();

                case lexing::TokenType::Immediate:
                case lexing::TokenType::Identifier:
                    return parseImmediate();

                case lexing::TokenType::String:
                    return parseString();
            }
        }

        ImmediatePtr parseImmediate()
        {
            if (current().getTokenType() == lexing::TokenType::Identifier)
            {
                return std::make_unique<LabelOperand>(consume().getText());
            }
            return std::make_unique<Immediate>(std::stoi(consume().getText()));
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

        StringPtr parseString()
        {
            return std::make_unique<String>(consume().getText());
        }

        TokenStream mTokens;
    };
}

#endif