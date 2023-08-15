// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H 1

#include "vasm/instruction/TwoOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"

namespace instruction
{
    template <unsigned char ModRM>
    struct LogicalInstructionImpl;
    template <auto... Ts>
    using LogicalInstruction = TwoOperandInstructionTemplate<LogicalInstructionImpl<Ts...>>;

    template <unsigned char ModRM>
    struct LogicalInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, LogicalInstruction<ModRM>& instruction)
        {
            // Assume lhs is a register for now
            Register* lhs = static_cast<Register*>(instruction.getLeft().get());

            if (Register* rhs = dynamic_cast<Register*>(instruction.getRight().get()))
            {
                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8))
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8 + 1))
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8 + 1))
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8 + 1))
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

    using AddInstruction = LogicalInstruction<0>;
    using AdcInstruction = LogicalInstruction<1>;
    using SbbInstruction = LogicalInstruction<2>;
    using OrInstruction  = LogicalInstruction<1>;
    using AndInstruction = LogicalInstruction<4>;
    using SubInstruction = LogicalInstruction<5>;
    using XorInstruction = LogicalInstruction<6>;
    using CmpInstruction = LogicalInstruction<7>;
}

#endif