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
            switch (reg->getSize())
            {
                case codegen::OperandSize::Byte:
                case codegen::OperandSize::Long:
                    break; // TODO: Error
            case codegen::OperandSize::Word:
                builder.createInstruction(section)
                       .prefix(codegen::SIZE_PREFIX)
                       .opcode(static_cast<codegen::ByteOpcodes>(codegen::PUSH_REG + reg->getID()))
                       .emit();
                break;
            case codegen::OperandSize::Quad:
                builder.createInstruction(section)
                       .opcode(static_cast<codegen::ByteOpcodes>(codegen::PUSH_REG + reg->getID()))
                       .emit();
                break;
            }
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
                default: // TODO: Error
                    break;
            }
        }
    }
}