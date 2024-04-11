// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H 1

#include "vasm/instruction/Instruction.h"
#include "vasm/instruction/Operand.h"

#include "vasm/codegen/Opcodes.h"

namespace instruction
{
    class TwoOperandInstruction : public Instruction
    {
    public:
        TwoOperandInstruction(OperandPtr left, OperandPtr right, codegen::OperandSize size, int lineNumber)
            : mLeft(std::move(left))
            , mRight(std::move(right))
            , mSize(size)
            , mLineNumber(lineNumber)
        { }

        virtual ~TwoOperandInstruction() { }

        int getLineNumber() const { return mLineNumber; }

    protected:
        OperandPtr mLeft;
        OperandPtr mRight;
        codegen::OperandSize mSize;
        int mLineNumber;
    };

    template <typename T>
    class TwoOperandInstructionTemplate : public TwoOperandInstruction
    {
    public:
        TwoOperandInstructionTemplate(OperandPtr left, OperandPtr right, codegen::OperandSize size, int lineNumber=-1) : TwoOperandInstruction(std::move(left), std::move(right), size, lineNumber) { }

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