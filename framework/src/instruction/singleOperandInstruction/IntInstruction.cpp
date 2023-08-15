// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/IntInstruction.h"

#include "vasm/instruction/operand/Immediate.h"

#include <cassert>

namespace instruction
{
    void IntInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, IntInstruction& instruction)
    {
        Immediate* vector = static_cast<Immediate*>(instruction.getOperand().get());

        builder.createInstruction(section)
               .opcode(codegen::INT)
               .immediate(vector->imm8())
               .emit();
    }
}