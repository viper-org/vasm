// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_INC_DEC_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_INC_DEC_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    template <InstructionStringBuilder Name, unsigned char modrm>
    struct IncDecInstructionImpl;
    template<InstructionStringBuilder Name, auto... Ts>
    using IncDecInstruction = SingleOperandInstructionTemplate<IncDecInstructionImpl<Name, Ts...>, Name>;

    template <InstructionStringBuilder Name, unsigned char modrm>
    struct IncDecInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, IncDecInstruction<Name, modrm>& instruction)
        {
            // Assume register for now
            Register* operand = static_cast<Register*>(instruction.getOperand().get());
            codegen::REX rex = codegen::REX::None;
            if (operand->getSize() == codegen::OperandSize::Quad) rex |= codegen::REX::W;
            if (operand->isExtended()) rex |= codegen::REX::B;

            switch (operand->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                        .prefix(rex)
                        .opcode(codegen::INC_DEC_REG8)
                        .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                        .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .prefix(codegen::SIZE_PREFIX)
                        .prefix(rex)
                        .opcode(codegen::INC_DEC_REG)
                        .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                        .prefix(rex)
                        .opcode(codegen::INC_DEC_REG)
                        .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                        .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .prefix(rex)
                        .opcode(codegen::INC_DEC_REG)
                        .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                        .emit();
                    break;
            }
        }
    };

    using IncInstruction = IncDecInstruction<"inc", 0>;
    using DecInstruction = IncDecInstruction<"dec", 1>;
}

#endif