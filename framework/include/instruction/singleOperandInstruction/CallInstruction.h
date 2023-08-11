// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_CALL_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_CALL_H 1

#include "instruction/SingleOperandInstruction.h"

namespace instruction
{
    class CallInstruction : public SingleOperandInstruction
    {
    public:
        CallInstruction(OperandPtr operand);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section);
    };
}

#endif