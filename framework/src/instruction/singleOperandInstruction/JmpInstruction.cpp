// Copyright 2023 solar-mist


#include "instruction/singleOperandInstruction/JmpInstruction.h"

#include "codegen/Opcodes.h"
#include "instruction/operand/Label.h"

namespace instruction
{
    void JmpInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, JmpInstruction& instruction)
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getOperand().get()))
        {
            builder.createInstruction(section)
                   .opcode(codegen::JMP_REL8)
                   .immediate(static_cast<unsigned char>(label->getValue(builder, section) - builder.getPosition(section) - 2))
                   .emit();
        }
        // TODO: Add call r/m
    }
}