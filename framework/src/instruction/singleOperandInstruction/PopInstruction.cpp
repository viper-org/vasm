// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/PopInstruction.h"

#include "vasm/codegen/Opcodes.h"
#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    void PopInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, PopInstruction& instruction)
    {
        if (Register* reg = dynamic_cast<Register*>(instruction.getOperand().get()))
        {
            switch (reg->getSize())
            {
                case codegen::OperandSize::Byte:
                case codegen::OperandSize::Long:
                    break; // TODO: Error
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::POP_REG + reg->getID()))
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::POP_REG + reg->getID()))
                           .emit();
                    break;
            }
        }
    }
}