// Copyright 2023 solar-mist


#include "instruction/singleOperandInstruction/PushInstruction.h"

#include "codegen/Opcodes.h"
#include "instruction/operand/Register.h"
#include "instruction/operand/Immediate.h"

namespace instruction
{
    PushInstruction::PushInstruction(OperandPtr operand)
        : SingleOperandInstruction(std::move(operand))
    {
    }

    void PushInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section) const
    {
        if (Register* reg = dynamic_cast<Register*>(mOperand.get()))
        {
            builder.createInstruction(section)
                .opcode(static_cast<codegen::ByteOpcodes>(codegen::PUSH_REG + reg->getID()))
                .emit();
        }
        else if (Immediate* imm = dynamic_cast<Immediate*>(mOperand.get()))
        {
            switch (imm->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                           .opcode(codegen::PUSH_IMM8)
                           .immediate(imm->imm8())
                           .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .opcode(codegen::PUSH_IMM)
                           .immediate(imm->imm16())
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::PUSH_IMM)
                           .immediate(imm->imm32())
                           .emit();
                    break;
                default:
                    break;
            }
        }
    }
}