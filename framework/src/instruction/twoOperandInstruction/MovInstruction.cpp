// Copyright 2023 solar-mist


#include "instruction/twoOperandInstruction/MovInstruction.h"
#include "codegen/Opcodes.h"
#include "codegen/builder/ModRM.h"
#include "instruction/TwoOperandInstruction.h"

#include "instruction/operand/Immediate.h"
#include "instruction/operand/Register.h"

namespace instruction
{
    MovInstruction::MovInstruction(OperandPtr left, OperandPtr right)
        : TwoOperandInstruction(std::move(left), std::move(right))
    {
    }

    void MovInstruction::emit(codegen::OpcodeBuilder& builder, codegen::Section section) const
    {
        // Assume LHS is a register for now
        Register* lhs = static_cast<Register*>(mLeft.get());
        
        if (Register* rhs = dynamic_cast<Register*>(mRight.get()))
        {
            switch (rhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                           .opcode(codegen::MOV_REG_REG8)
                           .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                           .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(codegen::MOV_REG_REG)
                           .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::MOV_REG_REG)
                           .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::MOV_REG_REG)
                           .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                           .emit();
                    break;
            }
        }
        else if (Immediate* rhs = dynamic_cast<Immediate*>(mRight.get()))
        {
            switch (lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM8 + lhs->getID()))
                           .immediate(rhs->imm8())
                           .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + lhs->getID()))
                           .immediate(rhs->imm16())
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + lhs->getID()))
                           .immediate(rhs->imm32())
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    if (rhs->getSize() == codegen::OperandSize::Quad)
                    {
                        builder.createInstruction(section)
                               .prefix(codegen::REX::W)
                               .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + lhs->getID()))
                               .immediate(rhs->imm64())
                               .emit();
                    }
                    else
                    {
                        builder.createInstruction(section)
                               .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + lhs->getID()))
                               .immediate(rhs->imm32())
                               .emit();
                    }
                    break;
            }
        }
    }
}