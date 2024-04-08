// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"

#include "vasm/codegen/Opcodes.h"

#include "vasm/error/ErrorMessages.h"

namespace instruction
{
    void PushInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, PushInstruction& instruction)
    {
        if (Register* reg = dynamic_cast<Register*>(instruction.getOperand().get()))
        {
            switch (reg->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.reportError(instruction.getLineNumber(), error::INV_OPCODE_OPERAND);
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .prefix(codegen::SIZE_PREFIX)
                        .opcode(static_cast<codegen::ByteOpcodes>(codegen::PUSH_REG + reg->getID()))
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .opcode(static_cast<codegen::ByteOpcodes>(codegen::PUSH_REG + reg->getID()))
                        .emit();
                    break;
                default:
                    break;
            }
        }
        else if (Immediate* imm = dynamic_cast<Immediate*>(instruction.getOperand().get()))
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
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::PUSH_IMM)
                           .immediate(imm->imm16())
                           .emit();
                    break;
                default:
                    builder.reportError(instruction.getLineNumber(), error::INV_OPCODE_OPERAND);
                    break;
            }
        }
    }
}