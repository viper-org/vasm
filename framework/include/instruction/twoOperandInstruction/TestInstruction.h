// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_TEST_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_TEST_H 1

#include "instruction/TwoOperandInstruction.h"

namespace instruction
{
    struct TestInstructionImpl;
    using TestInstruction = TwoOperandInstructionTemplate<TestInstructionImpl>;

    struct TestInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, TestInstruction& instruction);
    };
}

#endif