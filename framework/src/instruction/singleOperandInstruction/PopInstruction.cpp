// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/PopInstruction.h"

#include "vasm/codegen/Opcodes.h"
#include "vasm/instruction/operand/Register.h"

#include "vasm/error/ErrorMessages.h"

namespace instruction
{
    void PopInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, PopInstruction& instruction)
    {
        if (Register* reg = dynamic_cast<Register*>(instruction.getOperand().get()))
        {
            codegen::REX rex = codegen::REX::None;
            if (reg->isExtended()) rex = codegen::REX::B;

            switch (reg->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.reportError(instruction.getLineNumber(), error::INV_OPCODE_OPERAND);
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .prefix(rex)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::POP_REG + reg->getID()))
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(rex)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::POP_REG + reg->getID()))
                           .emit();
                    break;
                default:
                    break; // Unreachable
            }
        }
    }
}