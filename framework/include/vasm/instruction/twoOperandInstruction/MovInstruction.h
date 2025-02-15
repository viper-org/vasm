// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_MOV_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_MOV_H 1

#include "vasm/instruction/TwoOperandInstruction.h"

namespace instruction
{
    struct MovInstructionImpl;
    using MovInstruction = TwoOperandInstructionTemplate<MovInstructionImpl, "mov">;

    struct MovInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, MovInstruction& instruction);
    };


    struct MovZXInstructionImpl;
    using MovZXInstruction = TwoOperandInstructionTemplate<MovZXInstructionImpl, "movzx">;

    struct MovZXInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, MovZXInstruction& instruction);
    };

    struct MovSXInstructionImpl;
    using MovSXInstruction = TwoOperandInstructionTemplate<MovSXInstructionImpl, "movsx">;

    struct MovSXInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, MovSXInstruction& instruction);
    };
}

#endif