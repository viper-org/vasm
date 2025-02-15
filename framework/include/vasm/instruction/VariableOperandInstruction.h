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
        VariableOperandInstruction(OperandPtr left, OperandPtr right, OperandPtr third, codegen::OperandSize size, int lineNumber)
            : mLeft(std::move(left))
            , mRight(std::move(right))
            , mThird(std::move(third))
            , mSize(size)
            , mLineNumber(lineNumber)
        { }

        virtual ~VariableOperandInstruction() { }

        int getLineNumber() const { return mLineNumber; }

    protected:
        OperandPtr mLeft;
        OperandPtr mRight;
        OperandPtr mThird;
        codegen::OperandSize mSize;
        int mLineNumber;
    };

    template <typename T, InstructionStringBuilder Name>
    class VariableOperandInstructionTemplate : public VariableOperandInstruction
    {
    public:
        VariableOperandInstructionTemplate(OperandPtr left, OperandPtr right, OperandPtr third, codegen::OperandSize size=codegen::OperandSize::None, int lineNumber=-1)
            : VariableOperandInstruction(std::move(left), std::move(right), std::move(third), size, lineNumber) { }
        VariableOperandInstructionTemplate(OperandPtr left, OperandPtr right, codegen::OperandSize size=codegen::OperandSize::None, int lineNumber=-1)
            : VariableOperandInstruction(std::move(left), std::move(right), nullptr, size, lineNumber) { }
        VariableOperandInstructionTemplate(OperandPtr operand, codegen::OperandSize size = codegen::OperandSize::None, int lineNumber = -1)
            : VariableOperandInstruction(std::move(operand), nullptr, nullptr, size, lineNumber) { }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override
        {
            T::emit(builder, section, *this);
        }

        void print(std::ostream& stream) override
        {
            stream << '\t' << Name.mValue << " ";
            if (mLeft)
            {
                stream << mLeft->toString();
            }
            if (mRight)
            {
                stream << ", " << mRight->toString();
            }
            if (mThird)
            {
                stream << ", " << mThird->toString();
            }
        }

        OperandPtr& getLeft()
        {
            return mLeft;
        }

        OperandPtr& getRight()
        {
            return mRight;
        }

        OperandPtr& getThird()
        {
            return mThird;
        }

        OperandPtr& getOperand()
        {
            return mLeft;
        }

        bool isSingleOperand()
        {
            return mRight == nullptr;
        }

        bool isTwoOperand()
        {
            return mRight != nullptr && mThird == nullptr;
        }

        bool isThreeOperand()
        {
            return mThird != nullptr;
        }

        codegen::OperandSize getSize() const
        {
            return mSize;
        }
    };
}

#endif