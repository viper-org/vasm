// Copyright 2023 solar-mist


#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    Memory::Memory(RegisterPtr reg, std::optional<int> displacement)
        : mReg(std::move(reg))
        , mDisplacement(displacement)
    {
    }

    Register* Memory::getReg() const
    {
        return mReg.get();
    }

    std::optional<int> Memory::getDisplacement() const
    {
        return mDisplacement;
    }

    codegen::AddressingMode Memory::getAddressingMode() const
    {
        if (!mDisplacement)
        {
            return codegen::AddressingMode::RegisterIndirect;
        }
        if (mDisplacement <= INT8_MAX)
        {
            return codegen::AddressingMode::OneByteDisp;
        }
        return codegen::AddressingMode::FourByteDisp;
    }
}