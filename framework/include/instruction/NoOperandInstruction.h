// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_H 1

#include "instruction/Instruction.h"

namespace instruction
{
    class NoOperandInstruction : public Instruction
    {
    public:
        virtual ~NoOperandInstruction() { }
    };
}

#endif