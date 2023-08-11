// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_RET_H
#define VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_RET_H 1

#include "instruction/NoOperandInstruction.h"

namespace instruction
{
    class RetInstruction : public NoOperandInstruction
    {
    public:
        RetInstruction();

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section);
    };
}

#endif