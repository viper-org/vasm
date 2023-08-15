// Copyright 2023 solar-mist


#include "vasm/instruction/operand/Immediate.h"

namespace instruction
{
    Immediate::Immediate(unsigned long long value)
        : mValue(value)
    {
    }

    unsigned char Immediate::imm8() const
    {
        return mValue;
    }

    unsigned short Immediate::imm16() const
    {
        return mValue;
    }

    unsigned int Immediate::imm32() const
    {
        return mValue;
    }

    unsigned long Immediate::imm64() const
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
}