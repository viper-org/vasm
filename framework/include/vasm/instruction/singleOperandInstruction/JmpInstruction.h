// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JMP_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JMP_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

namespace instruction
{
    struct JmpInstructionImpl;
    using JmpInstruction = SingleOperandInstructionTemplate<JmpInstructionImpl, "jmp">;

    struct JmpInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, JmpInstruction& instruction);
    };
}

#endif