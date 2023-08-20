// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"

#include "vasm/codegen/Opcodes.h"
#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    void JmpInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, JmpInstruction& instruction)
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getOperand().get()))
        {
            auto value = label->getValue(builder, section);
            builder.createInstruction(section)
                   .opcode(codegen::JMP_REL8)
                   .immediate(static_cast<unsigned char>(value.first - builder.getPosition(section) - 2))
                   .emit();
            if (value.second || value.first == -1)
            {
                label->reloc(builder, section, codegen::OperandSize::Byte, -2);
            }
        }
        // TODO: Add call r/m
    }
}