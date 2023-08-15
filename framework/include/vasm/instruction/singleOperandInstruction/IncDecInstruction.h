// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_INC_DEC_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_INC_DEC_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    template <unsigned char modrm>
    struct IncDecInstructionImpl;
    template<auto... Ts>
    using IncDecInstruction = SingleOperandInstructionTemplate<IncDecInstructionImpl<Ts...>>;

    template <unsigned char modrm>
    struct IncDecInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, IncDecInstruction<modrm>& instruction)
        {
            // Assume register for now
            Register* operand = static_cast<Register*>(instruction.getOperand().get());

            switch (operand->getSize())
            {
                case codegen::OperandSize::Byte:
                    builder.createInstruction(section)
                           .opcode(codegen::INC_DEC_REG8)
                           .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                           .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                           .prefix(codegen::SIZE_PREFIX)
                           .opcode(codegen::INC_DEC_REG)
                           .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                           .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                           .opcode(codegen::INC_DEC_REG)
                           .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                           .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                           .prefix(codegen::REX::W)
                           .opcode(codegen::INC_DEC_REG)
                           .modrm(codegen::AddressingMode::RegisterDirect, modrm, operand->getID())
                           .emit();
                    break;
            }
        }
    };

    using IncInstruction = IncDecInstruction<0>;
    using DecInstruction = IncDecInstruction<1>;
}

#endif