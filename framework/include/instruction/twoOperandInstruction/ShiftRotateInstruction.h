// Copyright 2023 solar-mist

#include "instruction/operand/Immediate.h"
#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_SHIFT_ROTATE_H 
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_SHIFT_ROTATE_H 1

#include "instruction/TwoOperandInstruction.h"

#include "instruction/operand/Register.h"
#include "instruction/operand/Memory.h"

#include <cassert>

namespace instruction
{
    template <unsigned char ModRM>
    struct ShiftRotateInstructionImpl;
    template <auto... Ts>
    using ShiftRotateInstruction = TwoOperandInstructionTemplate<ShiftRotateInstructionImpl<Ts...>>;

    template <unsigned char ModRM>
    struct ShiftRotateInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, ShiftRotateInstruction<ModRM>& instruction)
        {
            Register* lhs;
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            std::optional<int> displacement;

            if (Memory* mem = dynamic_cast<Memory*>(instruction.getLeft().get()))
            {
                lhs = mem->getReg();
                addressingMode = mem->getAddressingMode();
                displacement = mem->getDisplacement();
            }
            else
            {
                lhs = static_cast<Register*>(instruction.getLeft().get());
            }

            if (Register* rhs = dynamic_cast<Register*>(instruction.getRight().get()))
            {
                assert(rhs->getID() == 1); // CL
                assert(rhs->getSize() == codegen::OperandSize::Byte);
                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                        builder.createInstruction(section)
                               .opcode(codegen::SHIFT_ROTATE_RM8_CL)
                               .modrm(addressingMode, ModRM, lhs->getID())
                               .displacement(displacement)
                               .emit();
                        break;
                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                               .prefix(codegen::SIZE_PREFIX)
                               .opcode(codegen::SHIFT_ROTATE_RM_CL)
                               .modrm(addressingMode, ModRM, lhs->getID())
                               .displacement(displacement)
                               .emit();
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                               .opcode(codegen::SHIFT_ROTATE_RM_CL)
                               .modrm(addressingMode, ModRM, lhs->getID())
                               .displacement(displacement)
                               .emit();
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                               .prefix(codegen::REX::W)
                               .opcode(codegen::SHIFT_ROTATE_RM_CL)
                               .modrm(addressingMode, ModRM, lhs->getID())
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
                                .opcode(codegen::SHIFT_ROTATE_RM8_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(codegen::SHIFT_ROTATE_RM_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(codegen::SHIFT_ROTATE_RM_1)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
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
                                .opcode(codegen::SHIFT_ROTATE_RM8_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                        case codegen::OperandSize::Word:
                            builder.createInstruction(section)
                                .prefix(codegen::SIZE_PREFIX)
                                .opcode(codegen::SHIFT_ROTATE_RM_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                        case codegen::OperandSize::Long:
                            builder.createInstruction(section)
                                .opcode(codegen::SHIFT_ROTATE_RM_IMM)
                                .modrm(addressingMode, ModRM, lhs->getID())
                                .displacement(displacement)
                                .immediate(rhs->imm8())
                                .emit();
                            break;
                        case codegen::OperandSize::Quad:
                            builder.createInstruction(section)
                                .prefix(codegen::REX::W)
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

    using RolInstruction = ShiftRotateInstruction<0>;
    using RorInstruction = ShiftRotateInstruction<1>;
    using RclInstruction = ShiftRotateInstruction<2>;
    using RcrInstruction = ShiftRotateInstruction<3>;
    using ShlInstruction = ShiftRotateInstruction<4>;
    using ShrInstruction = ShiftRotateInstruction<5>;
    using SalInstruction = ShiftRotateInstruction<6>;
    using SarInstruction = ShiftRotateInstruction<7>;
}

#endif