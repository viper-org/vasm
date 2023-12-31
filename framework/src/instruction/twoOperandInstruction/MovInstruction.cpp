// Copyright 2023 solar-mist


#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/codegen/Opcodes.h"
#include "vasm/codegen/builder/ModRM.h"
#include "vasm/instruction/TwoOperandInstruction.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"

#include <cassert>

namespace instruction
{
    void MovInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, MovInstruction& instruction)
    {
        codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
        std::optional<int> displacement;

        Register* regLhs = dynamic_cast<Register*>(instruction.getLeft().get());
        Memory*   memLhs = dynamic_cast<Memory*>(instruction.getLeft().get());
        if (memLhs)
        {
            addressingMode = memLhs->getAddressingMode();
            displacement = memLhs->getDisplacement();
            regLhs = memLhs->getReg();
        }
        
        Register* regRhs = dynamic_cast<Register*>(instruction.getRight().get());
        Memory*   memRhs = dynamic_cast<Memory*>(instruction.getRight().get());

        Register* reg;
        Register* rm;
        codegen::ByteOpcodes opcode8;
        codegen::ByteOpcodes opcode;
        if (memLhs || !memRhs)
        {
            reg = regRhs;
            rm = regLhs;
            opcode8 = codegen::MOV_RM_REG8;
            opcode = codegen::MOV_RM_REG;
        }
        else if (memRhs)
        {
            addressingMode = memRhs->getAddressingMode();
            displacement = memRhs->getDisplacement();
            regRhs = memRhs->getReg();

            reg = regLhs;
            rm = regRhs;
            opcode8 = codegen::MOV_REG_RM8;
            opcode = codegen::MOV_REG_RM;
        }

        if (regRhs || memRhs) // mov reg, reg OR mov reg, rm OR mov rm, reg
        {
            codegen::OperandSize size = regLhs->getSize();
            if (memLhs)
            {
                size = regRhs->getSize();
            }
            switch (size)
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                           .opcode(opcode8)
                           .modrm(addressingMode, reg->getID(), rm->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(opcode)
                           .modrm(addressingMode, reg->getID(), rm->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(opcode)
                           .modrm(addressingMode, reg->getID(), rm->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(opcode)
                           .modrm(addressingMode, reg->getID(), rm->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                default:
                    break;
            }
        }
        else if (Immediate* rhs = dynamic_cast<Immediate*>(instruction.getRight().get()))
        {
            int size = 0;
            if (memLhs)
            {
                switch (instruction.getSize())
                {
                    case codegen::OperandSize::Byte:
                        builder.createInstruction(section)
                               .opcode(codegen::MOV_RM_IMM8)
                               .modrm(addressingMode, 0, regLhs->getID())
                               .displacement(displacement)
                               .immediate(rhs->imm8())
                               .emit();
                        size = 1;
                        break;
                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                               .prefix(codegen::SIZE_PREFIX)
                               .opcode(codegen::MOV_RM_IMM)
                               .modrm(addressingMode, 0, regLhs->getID())
                               .displacement(displacement)
                               .immediate(rhs->imm16())
                               .emit();
                        size = 2;
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                               .opcode(codegen::MOV_RM_IMM)
                               .modrm(addressingMode, 0, regLhs->getID())
                               .displacement(displacement)
                               .immediate(rhs->imm32())
                               .emit();
                        size = 4;
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                               .prefix(codegen::REX::W)
                               .opcode(codegen::MOV_RM_IMM)
                               .modrm(addressingMode, 0, regLhs->getID())
                               .displacement(displacement)
                               .immediate(rhs->imm32())
                               .emit();
                        size = 4;
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch (regLhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                        builder.createInstruction(section)
                            .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM8 + regLhs->getID()))
                            .immediate(rhs->imm8())
                            .emit();
                        size = 1;
                        break;
                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                            .prefix(codegen::SIZE_PREFIX)
                            .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + regLhs->getID()))
                            .immediate(rhs->imm16())
                            .emit();
                        size = 2;
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                            .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + regLhs->getID()))
                            .immediate(rhs->imm32())
                            .emit();
                        size = 4;
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                                .prefix(codegen::REX::W)
                                .opcode(static_cast<codegen::ByteOpcodes>(codegen::MOV_REG_IMM + regLhs->getID()))
                                .immediate(rhs->imm64())
                                .emit();
                        size = 8;
                        break;
                    default:
                        break;
                }
            }
            if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getRight().get()))
            {
                label->reloc(builder, section, regLhs->getSize(), -size);
            }
        }
    }


    void MovZXInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, MovZXInstruction& instruction)
    {
        codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
        std::optional<int> displacement;

        Register* lhs = static_cast<Register*>(instruction.getLeft().get());

        Register* regRhs = dynamic_cast<Register*>(instruction.getRight().get());
        Memory*   memRhs = dynamic_cast<Memory*>(instruction.getRight().get());
        if (memRhs)
        {
            regRhs = memRhs->getReg();
            addressingMode = memRhs->getAddressingMode();
            displacement = memRhs->getDisplacement();
        }

        assert(regRhs->getSize() != codegen::OperandSize::Long);
        assert(regRhs->getSize() != codegen::OperandSize::Quad);

        if (regRhs->getSize() == codegen::OperandSize::Byte)
        {
            assert(lhs->getSize() != codegen::OperandSize::Byte);
            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // Unreachable
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(codegen::MOVZX8)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::MOVZX8)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::MOVZX8)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                default:
                    break;
            }
        }
        else // Word
        {
            assert(lhs->getSize() != codegen::OperandSize::Byte);
            assert(lhs->getSize() != codegen::OperandSize::Word);
            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // Unreachable
                case codegen::OperandSize::Word:
                    break; // Unreachable
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::MOVZX16)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::MOVZX16)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                default:
                    break;
            }
        }
    }

    void MovSXInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, MovSXInstruction& instruction)
    {
        codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
        std::optional<int> displacement;

        Register* lhs = static_cast<Register*>(instruction.getLeft().get());

        Register* regRhs = dynamic_cast<Register*>(instruction.getRight().get());
        Memory*   memRhs = dynamic_cast<Memory*>(instruction.getRight().get());
        if (memRhs)
        {
            regRhs = memRhs->getReg();
            addressingMode = memRhs->getAddressingMode();
            displacement = memRhs->getDisplacement();
        }

        assert(regRhs->getSize() != codegen::OperandSize::Quad);

        if (regRhs->getSize() == codegen::OperandSize::Byte)
        {
            assert(lhs->getSize() != codegen::OperandSize::Byte);
            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // Unreachable
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(codegen::MOVSX8)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::MOVSX8)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::MOVSX8)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                default:
                    break;
            }
        }
        else if (regRhs->getSize() == codegen::OperandSize::Word)
        {
            assert(lhs->getSize() != codegen::OperandSize::Byte);
            assert(lhs->getSize() != codegen::OperandSize::Word);
            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // Unreachable
                case codegen::OperandSize::Word:
                    break; // Unreachable
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::MOVSX16)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::MOVSX16)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                default:
                    break;
            }
        }
        else
        {
            assert(lhs->getSize() != codegen::OperandSize::Byte);
            assert(lhs->getSize() != codegen::OperandSize::Word);
            switch(lhs->getSize())
            {
                case codegen::OperandSize::Byte:
                    break; // Unreachable
                case codegen::OperandSize::Word:
                    break; // Unreachable
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::MOVSXD)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::MOVSXD)
                           .modrm(addressingMode, lhs->getID(), regRhs->getID())
                           .displacement(displacement)
                           .emit();
                    break;
                default:
                    break;
            }
        }
    }
}