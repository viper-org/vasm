// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_IMMEDIATE_H
#define VASM_INSTRUCTION_OPERAND_IMMEDIATE_H 1

#include "vasm/instruction/Operand.h"

#include "vasm/codegen/Opcodes.h"

namespace instruction
{
    class Immediate : public Operand
    {
    public:
        Immediate(unsigned long long value);

        unsigned char       imm8() const;
        unsigned short      imm16() const;
        unsigned int        imm32() const;
        unsigned long long  imm64() const;

        codegen::OperandSize getSize() const;

        std::unique_ptr<Operand> clone() override;

    private:
        unsigned long long mValue;
    };
    using ImmediatePtr = std::unique_ptr<Immediate>;
}

#endif