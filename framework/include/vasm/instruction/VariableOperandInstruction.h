// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_VARIABLE_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_VARIABLE_OPERAND_INSTRUCTION_H 1

#include "vasm/instruction/Instruction.h"
#include "vasm/instruction/Operand.h"

#include "vasm/codegen/Opcodes.h"

namespace instruction
{
    class VariableOperandInstruction : public Instruction
    {
    public:
        VariableOperandInstruction(OperandPtr left, OperandPtr right, codegen::OperandSize size, int lineNumber)
            : mLeft(std::move(left))
            , mRight(std::move(right))
            , mSize(size)
            , mLineNumber(lineNumber)
        { }

        virtual ~VariableOperandInstruction() { }

        int getLineNumber() const { return mLineNumber; }

    protected:
        OperandPtr mLeft;
        OperandPtr mRight;
        codegen::OperandSize mSize;
        int mLineNumber;
    };

    template <typename T>
    class VariableOperandInstructionTemplate : public VariableOperandInstruction
    {
    public:
        VariableOperandInstructionTemplate(OperandPtr left, OperandPtr right, codegen::OperandSize size=codegen::OperandSize::None, int lineNumber=-1) : VariableOperandInstruction(std::move(left), std::move(right), size, lineNumber) { }
        VariableOperandInstructionTemplate(OperandPtr operand, codegen::OperandSize size = codegen::OperandSize::None, int lineNumber = -1) : VariableOperandInstruction(std::move(operand), nullptr, size, lineNumber) { }

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

        OperandPtr& getOperand()
        {
            return mLeft;
        }

        bool isTwoOperand()
        {
            return mRight != nullptr;
        }

        codegen::OperandSize getSize() const
        {
            return mSize;
        }
    };
}

#endif