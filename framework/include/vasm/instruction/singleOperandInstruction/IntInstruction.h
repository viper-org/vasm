// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_INT_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_INT_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

namespace instruction
{
    struct IntInstructionImpl;
    using IntInstruction = SingleOperandInstructionTemplate<IntInstructionImpl>;

    struct IntInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, IntInstruction& instruction);
    };
}

#endif