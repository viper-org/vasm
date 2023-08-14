// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H 1

#include "instruction/Instruction.h"
#include "instruction/Operand.h"

#include "codegen/Opcodes.h"

namespace instruction
{
    class TwoOperandInstruction : public Instruction
    {
    public:
        TwoOperandInstruction(OperandPtr left, OperandPtr right, codegen::OperandSize size) : mLeft(std::move(left)), mRight(std::move(right)), mSize(size) { }

        virtual ~TwoOperandInstruction() { }

    protected:
        OperandPtr mLeft;
        OperandPtr mRight;
        codegen::OperandSize mSize;
    };

    template <typename T>
    class TwoOperandInstructionTemplate : public TwoOperandInstruction
    {
    public:
        TwoOperandInstructionTemplate(OperandPtr left, OperandPtr right, codegen::OperandSize size) : TwoOperandInstruction(std::move(left), std::move(right), size) { }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override
        {
            T::emit(builder, section, *this);
        }

        OperandPtr& getLeft()
        {
            return mLeft;
        }

        OperandPtr& getRight()
        {
            return mRight;
        }

        codegen::OperandSize getSize() const
        {
            return mSize;
        }
    };
}

#endif