// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_POP_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_POP_H 1

#include "instruction/SingleOperandInstruction.h"

namespace instruction
{
    class PopInstruction : public SingleOperandInstruction
    {
    public:
        PopInstruction(OperandPtr operand);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section);
    };
}

#endif