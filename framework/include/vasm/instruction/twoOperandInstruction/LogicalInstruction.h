// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_ADD_SUB_H 1

#include "vasm/instruction/TwoOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"

namespace instruction
{
    template <InstructionStringBuilder Name, unsigned char ModRM>
    struct LogicalInstructionImpl;
    template <InstructionStringBuilder Name, auto... Ts>
    using LogicalInstruction = TwoOperandInstructionTemplate<LogicalInstructionImpl<Name, Ts...>, Name>;

    template <InstructionStringBuilder Name, unsigned char ModRM>
    struct LogicalInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, LogicalInstruction<Name, ModRM>& instruction)
        {
            // Assume lhs is a register for now
            Register* lhs = static_cast<Register*>(instruction.getLeft().get());
            codegen::REX rex = codegen::REX::None;
            rex |= lhs->getRex();
            if (lhs->getSize() == codegen::OperandSize::Quad) rex |= codegen::REX::W;
            if (lhs->isExtended()) rex |= codegen::REX::B;

            if (Register* rhs = dynamic_cast<Register*>(instruction.getRight().get()))
            {
                rex |= rhs->getRex();
                if (rhs->getSize() == codegen::OperandSize::Quad) rex |= codegen::REX::W;
                if (rhs->isExtended()) rex |= codegen::REX::R;

                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8))
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .prefix(rex)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8 + 1))
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(static_cast<codegen::ByteOpcodes>(ModRM * 8 + 1))
                                .modrm(codegen::AddressingMode::RegisterDirect, rhs->getID(), lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(rex)
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
                                .prefix(rex)
                                .opcode(codegen::LOGICAL_REG8_IMM8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .prefix(rex)
                                .opcode(codegen::LOGICAL_REG_IMM8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::LOGICAL_REG_IMM8)
                                .immediate(rhs->imm8())
                                .modrm(codegen::AddressingMode::RegisterDirect, ModRM, lhs->getID())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(rex)
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

    using AddInstruction = LogicalInstruction<"add", 0>;
    using AdcInstruction = LogicalInstruction<"adc", 1>;
    using SbbInstruction = LogicalInstruction<"sbb", 2>;
    using OrInstruction  = LogicalInstruction<"or",  1>;
    using AndInstruction = LogicalInstruction<"and", 4>;
    using SubInstruction = LogicalInstruction<"sub", 5>;
    using XorInstruction = LogicalInstruction<"xor", 6>;
    using CmpInstruction = LogicalInstruction<"cmp", 7>;
}

#endif