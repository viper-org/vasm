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
    };

    using OperandPtr = std::unique_ptr<Operand>;
}

#endif