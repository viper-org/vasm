// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"

#include "vasm/codegen/Opcodes.h"
#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    void CallInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, CallInstruction& instruction)
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getOperand().get()))
        {
            auto value = label->getValue(builder, section);
            builder.createInstruction(section)
                   .opcode(codegen::CALL_REL32)
                   .immediate(static_cast<unsigned int>(value.first - builder.getPosition(section) - 5))
                   .emit();
            if (value.second || value.first == -1)
            {
                label->reloc(builder, section, codegen::OperandSize::Long, -4);
            }
        }
        // TODO: Add call r/m
    }
}