// Copyright 2023 solar-mist


#include "instruction/singleOperandInstruction/JmpInstruction.h"

#include "codegen/Opcodes.h"
#include "instruction/operand/Label.h"

namespace instruction
{
    JmpInstruction::JmpInstruction(OperandPtr operand)
        : SingleOperandInstruction(std::move(operand))
    {
    }

    void JmpInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section)
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(mOperand.get()))
        {
            builder.createInstruction(section)
                   .opcode(codegen::JMP_REL8)
                   .immediate(static_cast<unsigned char>(label->getValue(builder, section) - builder.getPosition(section) - 2))
                   .emit();
        }
        // TODO: Add call r/m
    }
}