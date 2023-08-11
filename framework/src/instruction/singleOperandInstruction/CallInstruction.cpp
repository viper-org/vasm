// Copyright 2023 solar-mist


#include "instruction/singleOperandInstruction/CallInstruction.h"

#include "codegen/Opcodes.h"
#include "instruction/operand/Label.h"

namespace instruction
{
    CallInstruction::CallInstruction(OperandPtr operand)
        : SingleOperandInstruction(std::move(operand))
    {
    }

    void CallInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section) const
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(mOperand.get()))
        {
            builder.createInstruction(section)
                   .opcode(codegen::CALL_REL32)
                   .immediate(static_cast<unsigned int>(label->getValue(builder) - builder.getPosition(section) - 5))
                   .emit();
        }
        // TODO: Add call r/m
    }
}