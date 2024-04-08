// Copyright 2023 solar-mist


#include "vasm/instruction/twoOperandInstruction/TestInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Immediate.h"

namespace instruction
{
    void TestInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, TestInstruction& instruction)
    {
        codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
        std::optional<int> displacement;

        Register* lhs = dynamic_cast<Register*>(instruction.getLeft().get());
        Memory* mem = dynamic_cast<Memory*>(instruction.getLeft().get());

        if (mem)
        {
            lhs = mem->getReg();
            addressingMode = mem->getAddressingMode();
            displacement = mem->getDisplacement();
        }

        if (Register* rhs = dynamic_cast<Register*>(instruction.getRight().get()))
        {
            switch (lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                            .opcode(codegen::TEST_RM_REG8)
                            .modrm(addressingMode, rhs->getID(), lhs->getID())
                            .displacement(displacement)
                            .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                            .prefix(codegen::SIZE_PREFIX)
                            .opcode(codegen::TEST_RM_REG)
                            .modrm(addressingMode, rhs->getID(), lhs->getID())
                            .displacement(displacement)
                            .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                            .opcode(codegen::TEST_RM_REG)
                            .modrm(addressingMode, rhs->getID(), lhs->getID())
                            .displacement(displacement)
                            .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                            .prefix(codegen::REX::W)
                            .opcode(codegen::TEST_RM_REG)
                            .modrm(addressingMode, rhs->getID(), lhs->getID())
                            .displacement(displacement)
                            .emit();
                    break;
                default:
                    break; // Unreachable
            }
        }
        else if (Immediate* imm = dynamic_cast<Immediate*>(instruction.getRight().get()))
        {
            switch (lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                            .opcode(codegen::TEST_RM_IMM8)
                            .modrm(addressingMode, codegen::ModRM::NullRegister, lhs->getID())
                            .immediate(imm->imm8())
                            .displacement(displacement)
                            .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                            .prefix(codegen::SIZE_PREFIX)
                            .opcode(codegen::TEST_RM_IMM)
                            .modrm(addressingMode, codegen::ModRM::NullRegister, lhs->getID())
                            .immediate(imm->imm16())
                            .displacement(displacement)
                            .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                            .opcode(codegen::TEST_RM_IMM)
                            .modrm(addressingMode, codegen::ModRM::NullRegister, lhs->getID())
                            .immediate(imm->imm32())
                            .displacement(displacement)
                            .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                            .prefix(codegen::REX::W)
                            .opcode(codegen::TEST_RM_IMM)
                            .modrm(addressingMode, codegen::ModRM::NullRegister, lhs->getID())
                            .immediate(imm->imm32())
                            .displacement(displacement)
                            .emit();
                    break;
                default:
                    break; // Unreachable
            }
        }
    }
}