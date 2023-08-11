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
            builder.createInstruction(section)
                .opcode(static_cast<codegen::ByteOpcodes>(codegen::POP_REG + reg->getID()))
                .emit();
        }
    }
}