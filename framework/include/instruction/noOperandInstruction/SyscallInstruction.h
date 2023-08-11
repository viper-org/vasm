// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_SYSCALL_H
#define VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_SYSCALL_H 1

#include "instruction/NoOperandInstruction.h"

namespace instruction
{
    class SyscallInstruction : public NoOperandInstruction
    {
    public:
        SyscallInstruction();

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) const;
    };
}

#endif