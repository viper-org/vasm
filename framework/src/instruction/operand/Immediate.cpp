// Copyright 2023 solar-mist


#include "vasm/instruction/operand/Immediate.h"

namespace instruction
{
    Immediate::Immediate(std::uint64_t value)
        : mValue(value)
    {
    }

    std::uint8_t Immediate::imm8() const
    {
        return mValue;
    }

    std::uint16_t Immediate::imm16() const
    {
        return mValue;
    }

    std::uint32_t Immediate::imm32() const
    {
        return mValue;
    }

    std::uint64_t Immediate::imm64() const
    {
        return mValue;
    }

    codegen::OperandSize Immediate::getSize() const
    {
        if (mValue <= UINT8_MAX)
        {
            return codegen::OperandSize::Byte;
        }
        else if (mValue <= UINT16_MAX)
        {
            return codegen::OperandSize::Word;
        }
        else if (mValue <= UINT32_MAX)
        {
            return codegen::OperandSize::Long;
        }
        else
        {
            return codegen::OperandSize::Quad;
        }
    }

    std::unique_ptr<Operand> Immediate::clone()
    {
        return std::make_unique<Immediate>(mValue);
    }

    std::string Immediate::toString()
    {
        return std::to_string(mValue);
    }
}