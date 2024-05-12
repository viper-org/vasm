// Copyright 2024 solar-mist


#include "vasm/instruction/variableOperandInstruction/IMulInstruction.h"

#include "vasm/codegen/Opcodes.h"
#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

namespace instruction
{
    void IMulInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, IMulInstruction& instruction)
    {
        if (instruction.isSingleOperand())
        {
            Register* operand;
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::OperandSize size;
            codegen::REX rex = codegen::REX::None;

            if (Memory* mem = dynamic_cast<Memory*>(instruction.getOperand().get()))
            {
                operand = mem->getBase();
                addressingMode = mem->getAddressingMode();
                displacement = mem->getDisplacement();
                sib = mem->getSIB();
                size = instruction.getSize();
                rex |= mem->getRex();
                if (size == codegen::OperandSize::Quad) rex |= codegen::REX::W;
            }
            else
            {
                operand = static_cast<Register*>(instruction.getOperand().get());
                size = operand->getSize();
                rex |= operand->getRex();
                if (operand->isExtended()) rex |= codegen::REX::B;
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
        else if (instruction.isTwoOperand())
        {
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::OperandSize size;
            codegen::REX rex = codegen::REX::None;

            Register* lhs = static_cast<Register*>(instruction.getLeft().get());
            rex |= lhs->getRex();
            if (lhs->isExtended()) rex |= codegen::REX::R;

            Register* regRhs = dynamic_cast<Register*>(instruction.getRight().get());
            Memory* memRhs = dynamic_cast<Memory*>(instruction.getRight().get());
            if (memRhs)
            {
                addressingMode = memRhs->getAddressingMode();
                displacement = memRhs->getDisplacement();
                sib = memRhs->getSIB();
                regRhs = memRhs->getBase();
                rex |= memRhs->getRex();
            }
            else
            {
                rex |= regRhs->getRex();
                if (regRhs->isExtended()) rex |= codegen::REX::B;
            }

            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // TODO: Error

                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .prefix(codegen::SIZE_PREFIX)
                        .prefix(rex)
                        .opcode(codegen::IMUL_REG_RM)
                        .modrm(addressingMode, lhs->getID(), regRhs->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                        .prefix(rex)
                        .opcode(codegen::IMUL_REG_RM)
                        .modrm(addressingMode, lhs->getID(), regRhs->getID())
                        .sib(sib)
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .prefix(rex)
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
        else
        {
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::OperandSize size;
            codegen::REX rex = codegen::REX::None;

            Register* lhs = static_cast<Register*>(instruction.getLeft().get());
            rex |= lhs->getRex();
            if (lhs->isExtended()) rex |= codegen::REX::R;

            Register* regRhs = dynamic_cast<Register*>(instruction.getRight().get());
            Memory* memRhs = dynamic_cast<Memory*>(instruction.getRight().get());
            Immediate* imm = dynamic_cast<Immediate*>(instruction.getThird().get());
            if (memRhs)
            {
                addressingMode = memRhs->getAddressingMode();
                displacement = memRhs->getDisplacement();
                sib = memRhs->getSIB();
                regRhs = memRhs->getBase();
                rex |= memRhs->getRex();
            }
            else
            {
                rex |= regRhs->getRex();
                if (regRhs->isExtended()) rex |= codegen::REX::B;
            }

            if (imm->imm8() < 0x7f)
            {
                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                        break; // TODO: Error

                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                            .prefix(codegen::SIZE_PREFIX)
                            .prefix(rex)
                            .opcode(codegen::IMUL_REG_RM_IMM8)
                            .modrm(addressingMode, lhs->getID(), regRhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .immediate(imm->imm8())
                            .emit();
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::IMUL_REG_RM_IMM8)
                            .modrm(addressingMode, lhs->getID(), regRhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .immediate(imm->imm8())
                            .emit();
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::IMUL_REG_RM_IMM8)
                            .modrm(addressingMode, lhs->getID(), regRhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .immediate(imm->imm8())
                            .emit();
                        break;

                    case codegen::OperandSize::None:
                        break;
                }
            }
            else
            {
                switch (lhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                        break; // TODO: Error

                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                            .prefix(codegen::SIZE_PREFIX)
                            .prefix(rex)
                            .opcode(codegen::IMUL_REG_RM_IMM)
                            .modrm(addressingMode, lhs->getID(), regRhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .immediate(imm->imm16())
                            .emit();
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::IMUL_REG_RM_IMM)
                            .modrm(addressingMode, lhs->getID(), regRhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .immediate(imm->imm32())
                            .emit();
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                            .prefix(rex)
                            .opcode(codegen::IMUL_REG_RM_IMM)
                            .modrm(addressingMode, lhs->getID(), regRhs->getID())
                            .sib(sib)
                            .displacement(displacement)
                            .immediate(imm->imm32())
                            .emit();
                        break;

                    case codegen::OperandSize::None:
                        break;
                }
            }
        }
    }
}