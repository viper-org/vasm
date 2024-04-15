// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_GRP4_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_GRP4_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

namespace instruction
{
    template <unsigned char ModRM>
    struct Grp4InstructionImpl;
    template <auto... Ts>
    using Grp4Instruction = SingleOperandInstructionTemplate<Grp4InstructionImpl<Ts...> >;

    template <unsigned char ModRM>
    struct Grp4InstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, Grp4Instruction<ModRM>& instruction)
        {
            Register* operand; 
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            std::optional<int> displacement;
            codegen::OperandSize size;

            if (Memory* mem = dynamic_cast<Memory*>(instruction.getOperand().get()))
            {
                operand = mem->getReg();
                addressingMode = mem->getAddressingMode();
                displacement = mem->getDisplacement();
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
                        .modrm(addressingMode, ModRM, operand->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .prefix(codegen::SIZE_PREFIX)
                        .opcode(codegen::GRP4_RM)
                        .modrm(addressingMode, ModRM, operand->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                        .opcode(codegen::GRP4_RM)
                        .modrm(addressingMode, ModRM, operand->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .prefix(codegen::REX::W)
                        .opcode(codegen::GRP4_RM)
                        .modrm(addressingMode, ModRM, operand->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::None:
                    break; // TODO: Error
            }
        }
    };

    using NotInstruction  = Grp4Instruction<2>;
    using NegInstruction  = Grp4Instruction<3>;
    using MulInstruction  = Grp4Instruction<4>;
    using IMulInstruction = Grp4Instruction<5>;
    using DivInstruction  = Grp4Instruction<6>;
    using IDivInstruction = Grp4Instruction<7>;
}

#endif