// Copyright 2023 solar-mist


#include "instruction/noOperandInstruction/SyscallInstruction.h"

namespace instruction
{
    SyscallInstruction::SyscallInstruction()
    {
    }

    void SyscallInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section)
    {
        builder.createInstruction(section)
               .opcode(codegen::SYSCALL)
               .emit();
    }
}