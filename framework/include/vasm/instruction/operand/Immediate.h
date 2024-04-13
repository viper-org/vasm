// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_IMMEDIATE_H
#define VASM_INSTRUCTION_OPERAND_IMMEDIATE_H 1

#include "vasm/instruction/Operand.h"

#include "vasm/codegen/Opcodes.h"

#include <cstdint>

namespace instruction
{
    class Immediate : public Operand
    {
    public:
        Immediate(std::uint64_t value);

        std::uint8_t  imm8() const;
        std::uint16_t imm16() const;
        std::uint32_t imm32() const;
        std::uint64_t imm64() const;

        codegen::OperandSize getSize() const;

        std::unique_ptr<Operand> clone() override;

    private:
        std::uint64_t mValue;
    };
    using ImmediatePtr = std::unique_ptr<Immediate>;
}

#endif