// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H 1

#include "instruction/TwoOperandInstruction.h"

#include "instruction/operand/Register.h"
#include "instruction/operand/Immediate.h"

namespace instruction
{
    template <codegen::ByteOpcodes RegReg8,
              codegen::ByteOpcodes RegReg,
              unsigned char ModRM>
    struct LogicalInstructionImpl;
    template <auto... Ts>
    using LogicalInstruction = TwoOperandInstructionTemplate<LogicalInstructionImpl<Ts...>>;

    template <codegen::ByteOpcodes RegReg8,
              codegen::ByteOpcodes RegReg,
              unsigned char ModRM>
    struct LogicalInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, LogicalInstruction<RegReg8,
                                                                                                      RegReg,
                                                                                                      ModRM>& instruction)
        {
            // Assume lhs is a register for now
            Register* lhs = static_cast<Register*>(instruction.getLeft().get());

            if (Register* rhs = dynamic_cast<Register*>(instruction.getRight().get()))
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
            else if (Immediate* rhs = dynamic_cast<Immediate*>(instruction.getRight().get()))
            {
                if (rhs->getSize() == codegen::OperandSize::Byte)
                {
                    switch (lhs->getSize())
                    {
                        case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .opcode(codegen::LOGICAL_REG8_IMM8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(codegen::LOGICAL_REG_IMM8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(codegen::LOGICAL_REG_IMM8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(codegen::LOGICAL_REG_IMM8)
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
                                .opcode(codegen::LOGICAL_REG_IMM)
                                .immediate(rhs->imm16())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(codegen::LOGICAL_REG_IMM)
                                .immediate(rhs->imm32())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(codegen::LOGICAL_REG_IMM)
                                .immediate(rhs->imm32())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                    }
                }
            }
        }
    };

    using AddInstruction = LogicalInstruction<codegen::ADD_REG_REG8, 
                                              codegen::ADD_REG_REG, 0>;

    using OrInstruction  = LogicalInstruction<codegen::OR_REG_REG8, 
                                              codegen::OR_REG_REG,  1>;                                              

    using AndInstruction = LogicalInstruction<codegen::AND_REG_REG8,
                                              codegen::AND_REG_REG, 4>;

    using SubInstruction = LogicalInstruction<codegen::SUB_REG_REG8, 
                                              codegen::SUB_REG_REG, 5>;

    using XorInstruction = LogicalInstruction<codegen::XOR_REG_REG8, 
                                              codegen::XOR_REG_REG, 6>;

    using CmpInstruction = LogicalInstruction<codegen::CMP_REG_REG8,
                                              codegen::CMP_REG_REG, 7>;
}

#endif