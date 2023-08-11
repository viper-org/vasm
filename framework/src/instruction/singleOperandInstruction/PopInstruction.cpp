// Copyright 2023 solar-mist


#include "instruction/singleOperandInstruction/PopInstruction.h"

#include "codegen/Opcodes.h"
#include "instruction/operand/Register.h"

namespace instruction
{
    PopInstruction::PopInstruction(OperandPtr operand)
        : SingleOperandInstruction(std::move(operand))
    {
    }

    void PopInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section) const
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