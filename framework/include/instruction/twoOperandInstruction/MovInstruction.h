// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_MOV_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_MOV_H 1

#include "instruction/TwoOperandInstruction.h"

namespace instruction
{
    class MovInstruction : public TwoOperandInstruction
    {
    public:
        MovInstruction(OperandPtr left, OperandPtr right);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) const;
    };
}

#endif