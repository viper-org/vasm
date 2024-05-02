// Copyright 2024 solar-mist


#include "vasm/instruction/variableOperandInstruction/IMulInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

namespace instruction
{
    void IMulInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, IMulInstruction& instruction)
    {
        if (!instruction.isTwoOperand())
        {
            Register* operand;
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::OperandSize size;

            if (Memory* mem = dynamic_cast<Memory*>(instruction.getOperand().get()))
            {
                operand = mem->getBase();
                addressingMode = mem->getAddressingMode();
                displacement = mem->getDisplacement();
                sib = mem->getSIB();
                size = instruction.getSize();
            }
            else
            {
                operand = static_cast<Register*>(instruction.getOperand().get());
                size = operand->getSize();
            }

            switch(size)
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                        .opcode(codegen::GRP4_RM8)
                        .modrm(addressingMode, 5, operand->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .prefix(codegen::SIZE_PREFIX)
                        .opcode(codegen::GRP4_RM)
                        .modrm(addressingMode, 5, operand->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                        .opcode(codegen::GRP4_RM)
                        .modrm(addressingMode, 5, operand->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .prefix(codegen::REX::W)
                        .opcode(codegen::GRP4_RM)
                        .modrm(addressingMode, 5, operand->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::None:
                    break; // TODO: Error
            }
        }
        else
        {
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::OperandSize size;

            Register* lhs = static_cast<Register*>(instruction.getLeft().get());
            Register* regRhs = dynamic_cast<Register*>(instruction.getRight().get());
            Memory* memRhs = dynamic_cast<Memory*>(instruction.getRight().get());
            if (memRhs)
            {
                addressingMode = memRhs->getAddressingMode();
                displacement = memRhs->getDisplacement();
                sib = memRhs->getSIB();
                regRhs = memRhs->getBase();
            }

            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // TODO: Error

                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .prefix(codegen::SIZE_PREFIX)
                        .opcode(codegen::IMUL_REG_RM)
                        .modrm(addressingMode, lhs->getID(), regRhs->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                        .opcode(codegen::IMUL_REG_RM)
                        .modrm(addressingMode, lhs->getID(), regRhs->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .prefix(codegen::REX::W)
                        .opcode(codegen::IMUL_REG_RM)
                        .modrm(addressingMode, lhs->getID(), regRhs->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;

                case codegen::OperandSize::None:
                    break; // Unreachable
            }
        }
    }
}