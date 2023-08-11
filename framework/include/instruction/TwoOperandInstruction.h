// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_H 1

#include "instruction/Instruction.h"
#include "instruction/Operand.h"

namespace instruction
{
    class TwoOperandInstruction : public Instruction
    {
    public:
        TwoOperandInstruction(OperandPtr left, OperandPtr right) : mLeft(std::move(left)), mRight(std::move(right)) { }

        virtual ~TwoOperandInstruction() { }

    protected:
        OperandPtr mLeft;
        OperandPtr mRight;
    };

    template <typename T>
    class TwoOperandInstructionTemplate : public TwoOperandInstruction
    {
    public:
        TwoOperandInstructionTemplate(OperandPtr left, OperandPtr right) : TwoOperandInstruction(std::move(left), std::move(right)) { }

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
    };
}

#endif