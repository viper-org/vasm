// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_DECL_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_DECL_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/String.h"

namespace instruction
{
    template <codegen::OperandSize Size, InstructionStringBuilder Name>
    struct DeclInstructionImpl;
    template <codegen::OperandSize Size, InstructionStringBuilder Name>
    using DeclInstruction = SingleOperandInstructionTemplate<DeclInstructionImpl<Size, Name>, Name>;

    template <codegen::OperandSize Size, InstructionStringBuilder Name>
    struct DeclInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, DeclInstruction<Size, Name>& instruction)
        {
            if (Immediate* imm = dynamic_cast<Immediate*>(instruction.getOperand().get()))
            {
                switch(Size)
                {
                    case codegen::OperandSize::Byte:
                        builder.createInstruction(section)
                               .immediate(imm->imm8())
                               .emit();
                        break;
                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                               .immediate(imm->imm16())
                               .emit();
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                               .immediate(imm->imm32())
                               .emit();
                        break;
                    case codegen::OperandSize::Quad:
                        builder.createInstruction(section)
                               .immediate(imm->imm64())
                               .emit();
                        break;
                }
            }
            else if (String* str = dynamic_cast<String*>(instruction.getOperand().get()))
            {
                builder.createInstruction(section)
                       .string(str->getText())
                       .emit();
            }
        }
    };

    using DbInstruction = DeclInstruction<codegen::OperandSize::Byte, "db">;
    using DwInstruction = DeclInstruction<codegen::OperandSize::Word, "dw">;
    using DlInstruction = DeclInstruction<codegen::OperandSize::Long, "dl">;
    using DqInstruction = DeclInstruction<codegen::OperandSize::Quad, "dq">;
}

#endif