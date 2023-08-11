// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_H 1

#include "instruction/Instruction.h"
#include "instruction/Operand.h"

namespace instruction
{
    class SingleOperandInstruction : public Instruction
    {
    public:
        SingleOperandInstruction(OperandPtr operand) : mOperand(std::move(operand)) { }

        virtual ~SingleOperandInstruction() { }

    protected:
        OperandPtr mOperand;
    };
}

#endif