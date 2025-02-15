// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_SHIFT_ROTATE_H 
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_SHIFT_ROTATE_H 1

#include "vasm/instruction/TwoOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Immediate.h"

#include <cassert>

namespace instruction
{
    template <InstructionStringBuilder Name, unsigned char ModRM>
    struct ShiftRotateInstructionImpl;
    template <InstructionStringBuilder Name, auto... Ts>
    using ShiftRotateInstruction = TwoOperandInstructionTemplate<ShiftRotateInstructionImpl<Name, Ts...>, Name>;

    template <InstructionStringBuilder Name, unsigned char ModRM>
    struct ShiftRotateInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, ShiftRotateInstruction<Name, ModRM>& instruction)
        {
            Register* lhs;
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::REX rex = codegen::REX::None;
            codegen::OperandSize size;

            if (Memory* mem = dynamic_cast<Memory*>(instruction.getLeft().get()))
            {
                lhs = mem->getBase();
                addressingMode = mem->getAddressingMode();
                displacement = mem->getDisplacement();
                sib = mem->getSIB();
                size = instruction.getSize();
                rex |= mem->getRex();
            }
            else
            {
                lhs = static_cast<Register*>(instruction.getLeft().get());
                size = lhs->getSize();
                if (lhs->isExtended()) rex |= codegen::REX::B;
                rex |= lhs->getRex();
            }
            if (size == codegen::OperandSize::Quad) rex |= codegen::REX::W;

            if (Register* rhs = dynamic_cast<Register*>(instruction.getRight().get()))
            {
                assert(rhs->getID() == 1); // CL
                assert(rhs->getSize() == codegen::OperandSize::Byte);
                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::SHIFT_ROTATE_RM8_CL)
                            .modrm(addressingMode, ModRM, lhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .emit();
                        break;
                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                            .prefix(codegen::SIZE_PREFIX)
                            .prefix(rex)
                            .opcode(codegen::SHIFT_ROTATE_RM_CL)
                            .modrm(addressingMode, ModRM, lhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .emit();
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::SHIFT_ROTATE_RM_CL)
                            .modrm(addressingMode, ModRM, lhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .emit();
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::SHIFT_ROTATE_RM_CL)
                            .modrm(addressingMode, ModRM, lhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .emit();
                        break;
                }
            }
            else if (Immediate* rhs = dynamic_cast<Immediate*>(instruction.getRight().get()))
            {
                if (rhs->imm8() == 1)
                {
                    switch (lhs->getSize())
                    {
                        case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM8_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                    }
                }
                else
                {
                    switch (lhs->getSize())
                    {
                        case codegen::OperandSize::Byte:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM8_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(rex)
                                .opcode(codegen::SHIFT_ROTATE_RM_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                    }
                }
            }
        }
    };

    using RolInstruction = ShiftRotateInstruction<"rol", 0>;
    using RorInstruction = ShiftRotateInstruction<"ror", 1>;
    using RclInstruction = ShiftRotateInstruction<"rcl", 2>;
    using RcrInstruction = ShiftRotateInstruction<"rcr", 3>;
    using ShlInstruction = ShiftRotateInstruction<"shl", 4>;
    using ShrInstruction = ShiftRotateInstruction<"shr", 5>;
    using SalInstruction = ShiftRotateInstruction<"sal", 6>;
    using SarInstruction = ShiftRotateInstruction<"sar", 7>;
}

#endif