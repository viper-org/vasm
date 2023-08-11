// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H 1

#include "instruction/Instruction.h"
#include "instruction/Operand.h"

namespace instruction
{
    class TwoOperandInstruction : public Instruction
    {
    public:
        TwoOperandInstruction(OperandPtr left, OperandPtr right) : mLeft(std::move(left)), mRight(std::move(right)) { }

        virtual ~TwoOperandInstruction() { }

    protected:
        OperandPtr mLeft;
        OperandPtr mRight;
    };
}

#endif