// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_XCHG_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_XCHG_H 1

#include "vasm/instruction/TwoOperandInstruction.h"

namespace instruction
{
    struct XchgInstructionImpl;
    using XchgInstruction = TwoOperandInstructionTemplate<XchgInstructionImpl>;

    struct XchgInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, XchgInstruction& instruction);
    };
}

#endif