// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_PUSH_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_PUSH_H 1

#include "instruction/SingleOperandInstruction.h"

namespace instruction
{
    class PushInstruction : public SingleOperandInstruction
    {
    public:
        PushInstruction(OperandPtr operand);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) const;
    };
}

#endif