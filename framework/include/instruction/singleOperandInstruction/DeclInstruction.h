// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_DECL_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_DECL_H 1

#include "instruction/SingleOperandInstruction.h"

#include "instruction/operand/Immediate.h"
#include "instruction/operand/String.h"

namespace instruction
{
    template <codegen::OperandSize _mSize>
    class DeclInstruction : public SingleOperandInstruction
    {
    public:
        DeclInstruction(OperandPtr operand)
            : SingleOperandInstruction(std::move(operand))
        {
        }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section)
        {
            if (Immediate* imm = dynamic_cast<Immediate*>(mOperand.get()))
            {
                switch(_mSize)
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
            else if (String* str = dynamic_cast<String*>(mOperand.get()))
            {
                builder.createInstruction(section)
                       .string(str->getText())
                       .emit();
            }
        }
    };
}

#endif