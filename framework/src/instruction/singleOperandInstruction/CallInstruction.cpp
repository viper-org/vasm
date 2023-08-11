// Copyright 2023 solar-mist


#include "instruction/singleOperandInstruction/CallInstruction.h"

#include "codegen/Opcodes.h"
#include "instruction/operand/Label.h"

namespace instruction
{
    void CallInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, CallInstruction& instruction)
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getOperand().get()))
        {
            builder.createInstruction(section)
                   .opcode(codegen::CALL_REL32)
                   .immediate(static_cast<unsigned int>(label->getValue(builder, section) - builder.getPosition(section) - 5))
                   .emit();
        }
        // TODO: Add call r/m
    }
}