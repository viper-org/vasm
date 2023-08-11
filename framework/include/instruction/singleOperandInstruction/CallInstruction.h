// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_CALL_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_CALL_H 1

#include "instruction/SingleOperandInstruction.h"

namespace instruction
{
    struct CallInstructionImpl;
    using CallInstruction = SingleOperandInstructionTemplate<CallInstructionImpl>;

    struct CallInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, CallInstruction& instruction);
    };
}

#endif