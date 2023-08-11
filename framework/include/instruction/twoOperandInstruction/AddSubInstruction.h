// Copyright 2023 solar-mist

#include "codegen/Opcodes.h"
#include "codegen/builder/ModRM.h"
#include "instruction/operand/Immediate.h"
#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H 1

#include "instruction/TwoOperandInstruction.h"

#include "instruction/operand/Register.h"

namespace instruction
{
    template <codegen::ByteOpcodes RegReg8,
              codegen::ByteOpcodes RegReg,
              codegen::ByteOpcodes Reg8Imm8,
              codegen::ByteOpcodes RegImm8,
              codegen::ByteOpcodes RegImm,
              unsigned char ModRM>
    class AddSubInstruction : public TwoOperandInstruction
    {
    public:
        AddSubInstruction(OperandPtr left, OperandPtr right)
            : TwoOperandInstruction(std::move(left), std::move(right))
        {
        }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) const
        {
            // Assume lhs is a register for now
            Register* lhs = static_cast<Register*>(mLeft.get());

            if (Register* rhs = dynamic_cast<Register*>(mRight.get()))
            {
                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .opcode(RegReg8)
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(RegReg)
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(RegReg)
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(RegReg)
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                }
            }
            else if (Immediate* rhs = dynamic_cast<Immediate*>(mRight.get()))
            {
                if (rhs->getSize() == codegen::OperandSize::Byte)
                {
                    switch (lhs->getSize())
                    {
                        case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .opcode(Reg8Imm8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(RegImm8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(RegImm8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(RegImm8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                    }
                }
                else
                {
                    switch (lhs->getSize())
                    {
                        case codegen::OperandSize::Byte: // TODO: Error
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(RegImm)
                                .immediate(rhs->imm16())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(RegImm)
                                .immediate(rhs->imm32())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(RegImm)
                                .immediate(rhs->imm32())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                    }
                }
            }
        }
    };

    using AddInstruction = AddSubInstruction<codegen::ADD_REG_REG8, 
                                             codegen::ADD_REG_REG, 
                                             codegen::ADD_REG8_IMM8, 
                                             codegen::ADD_REG_IMM8, 
                                             codegen::ADD_REG_IMM, 0>;

    using SubInstruction = AddSubInstruction<codegen::SUB_REG_REG8, 
                                             codegen::SUB_REG_REG, 
                                             codegen::SUB_REG8_IMM8, 
                                             codegen::SUB_REG_IMM8, 
                                             codegen::SUB_REG_IMM, 5>;
}

#endif