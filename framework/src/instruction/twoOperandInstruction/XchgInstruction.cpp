// Copyright 2023 solar-mist


#include "vasm/instruction/twoOperandInstruction/XchgInstruction.h"

#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    void XchgInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, XchgInstruction& instruction)
    {
        // Assume registers for now
        Register* lhs = static_cast<Register*>(instruction.getLeft().get());
        Register* rhs = static_cast<Register*>(instruction.getRight().get());

        codegen::REX rex = codegen::REX::None;
        rex |= lhs->getRex();
        rex |= rhs->getRex();
        if (lhs->isExtended()) rex |= codegen::REX::R;
        if (rhs->isExtended()) rex |= codegen::REX::B;

        switch (lhs->getSize())
        {
            case codegen::OperandSize::Byte:
                builder.createInstruction(section)
                    .prefix(rex)
                    .opcode(codegen::XCHG_REG_REG8)
                    .modrm(codegen::AddressingMode::RegisterDirect, lhs->getID(), rhs->getID())
                    .emit();
                break;
            case codegen::OperandSize::Word:
                builder.createInstruction(section)
                    .prefix(codegen::SIZE_PREFIX)
                    .prefix(rex)
                    .opcode(codegen::XCHG_REG_REG)
                    .modrm(codegen::AddressingMode::RegisterDirect, lhs->getID(), rhs->getID())
                    .emit();
                break;
            case codegen::OperandSize::Long:
                builder.createInstruction(section)
                    .prefix(rex)
                    .opcode(codegen::XCHG_REG_REG)
                    .modrm(codegen::AddressingMode::RegisterDirect, lhs->getID(), rhs->getID())
                    .emit();
                break;
            case codegen::OperandSize::Quad:
                builder.createInstruction(section)
                    .prefix(rex)
                    .opcode(codegen::XCHG_REG_REG)
                    .modrm(codegen::AddressingMode::RegisterDirect, lhs->getID(), rhs->getID())
                    .emit();
                break;
            default:
                break; // Unreachable
        }
    }
}