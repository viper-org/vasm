// Copyright 2023 solar-mist


#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include "vasm/codegen/Opcodes.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    void LeaInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, LeaInstruction& instruction)
    {
        Register* lhs = static_cast<Register*>(instruction.getLeft().get());
        Memory* rhs   = static_cast<Memory*>(instruction.getRight().get());

        codegen::AddressingMode addressingMode = rhs->getAddressingMode();

        switch (lhs->getSize())
        {
            case codegen::OperandSize::Byte:
                break; // TODO: Error
            case codegen::OperandSize::Word:
                builder.createInstruction(section)
                       .prefix(codegen::SIZE_PREFIX)
                       .opcode(codegen::LEA)
                       .modrm(addressingMode, lhs->getID(), rhs->getReg()->getID())
                       .displacement(rhs->getDisplacement())
                       .emit();
                break;
            case codegen::OperandSize::Long:
                builder.createInstruction(section)
                       .opcode(codegen::LEA)
                       .modrm(addressingMode, lhs->getID(), rhs->getReg()->getID())
                       .displacement(rhs->getDisplacement())
                       .emit();
                break;
            case codegen::OperandSize::Quad:
                builder.createInstruction(section)
                       .prefix(codegen::REX::W)
                       .opcode(codegen::LEA)
                       .modrm(addressingMode, lhs->getID(), rhs->getReg()->getID())
                       .displacement(rhs->getDisplacement())
                       .emit();
                break;
        }
    }
}