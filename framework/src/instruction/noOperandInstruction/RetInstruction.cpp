// Copyright 2023 solar-mist


#include "instruction/noOperandInstruction/RetInstruction.h"

namespace instruction
{
    RetInstruction::RetInstruction()
    {
    }

    void RetInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section)
    {
        builder.createInstruction(section)
               .opcode(codegen::RET)
               .emit();
    }
}