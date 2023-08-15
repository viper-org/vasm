// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_POP_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_POP_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

namespace instruction
{
    struct PopInstructionImpl;
    using PopInstruction = SingleOperandInstructionTemplate<PopInstructionImpl>;
    
    struct PopInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, PopInstruction& instruction);
    };
}

#endif