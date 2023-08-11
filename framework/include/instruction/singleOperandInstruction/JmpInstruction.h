// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JMP_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JMP_H 1

#include "instruction/SingleOperandInstruction.h"

namespace instruction
{
    class JmpInstruction : public SingleOperandInstruction
    {
    public:
        JmpInstruction(OperandPtr operand);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section);
    };
}

#endif