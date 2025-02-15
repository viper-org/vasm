// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_VARIABLE_OPERAND_INSTRUCTION_LEA_H
#define VASM_INSTRUCTION_VARIABLE_OPERAND_INSTRUCTION_LEA_H 1

#include "vasm/instruction/VariableOperandInstruction.h"

namespace instruction
{
    struct IMulInstructionImpl;
    using IMulInstruction = VariableOperandInstructionTemplate<IMulInstructionImpl, "imul">;

    struct IMulInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, IMulInstruction& instruction);
    };
}

#endif