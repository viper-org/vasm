// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_H
#define VASM_INSTRUCTION_OPERAND_H 1

#include <memory>

namespace instruction
{
    class Operand
    {
    public:
        virtual ~Operand() { }

        virtual std::unique_ptr<Operand> clone() = 0;
    };

    using OperandPtr = std::unique_ptr<Operand>;
}

#endif