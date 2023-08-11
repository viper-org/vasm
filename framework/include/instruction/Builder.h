// Copyright 2023 solar-mist

#include "codegen/Opcodes.h"
#ifndef VASM_INSTRUCTION_BUILDER_H
#define VASM_INSTRUCTION_BUILDER_H 1

#include "instruction/NoOperandInstruction.h"
#include "instruction/SingleOperandInstruction.h"

#include "instruction/operand/Immediate.h"
#include "instruction/operand/Register.h"

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
                return std::make_unique<T>(ParseOperand());
            }
        }

    private:
        OperandPtr ParseOperand()
        {
            switch (mTokens.tokens->at(*mTokens.position).getTokenType())
            {
                case lexing::TokenType::Register:
                    return ParseRegister();

                case lexing::TokenType::Immediate:
                    return ParseImmediate();
            }
        }

        ImmediatePtr ParseImmediate()
        {
            return std::make_unique<Immediate>(std::stoi(mTokens.tokens->at((*mTokens.position)++).getText()));
        }

        RegisterPtr ParseRegister()
        {
            constexpr int REGISTERS_PER_ENCODING = 4;

            unsigned long long index;
            for (index = 0; index < static_cast<unsigned long long>(codegen::Registers.size()); index++)
            {
                if (codegen::Registers[index] == mTokens.tokens->at(*mTokens.position).getText())
                {
                    break;
                }
            }
            *mTokens.position += 1;

            return std::make_unique<Register>(index / REGISTERS_PER_ENCODING, static_cast<codegen::OperandSize>(index % REGISTERS_PER_ENCODING));
        }

        TokenStream mTokens;
    };
}

#endif