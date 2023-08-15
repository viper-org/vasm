// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_H 1

#include "vasm/instruction/Instruction.h"
#include "vasm/instruction/Operand.h"

namespace instruction
{
    class SingleOperandInstruction : public Instruction
    {
    public:
        SingleOperandInstruction(OperandPtr operand) : mOperand(std::move(operand)) { }

        virtual ~SingleOperandInstruction() { }

    protected:
        OperandPtr mOperand;
    };

    template <typename T>
    class SingleOperandInstructionTemplate : public SingleOperandInstruction
    {
    public:
        SingleOperandInstructionTemplate(OperandPtr operand) : SingleOperandInstruction(std::move(operand)) { }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override
        {
            T::emit(builder, section, *this);
        }

        OperandPtr& getOperand()
        {
            return mOperand;
        }
    };
}

#endif