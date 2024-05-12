// Copyright 2023 solar-mist


#include "vasm/instruction/operand/Register.h"

#include <algorithm>

namespace instruction
{
    Register::Register(unsigned char id, codegen::OperandSize size, bool rex, bool extended)
        : mID(id)
        , mSize(size)
        , mRex(rex)
        , mExtended(extended)
    {
        if (mID >= 4 && mID <= 7 && mSize == codegen::OperandSize::Byte)
            rex = true;
    }

    unsigned char Register::getID() const
    {
        return mID;
    }

    codegen::OperandSize Register::getSize() const
    {
        return mSize;
    }

    bool Register::isExtended() const
    {
        return mExtended;
    }

    RegisterPtr Register::Get(std::string_view name)
    {
        constexpr int REGISTERS_PER_ENCODING = 4;

        auto it = std::find(codegen::Registers.begin(), codegen::Registers.end(), name);
        if (it != codegen::Registers.end())
        {
            std::size_t index = it - codegen::Registers.begin();
            return std::make_unique<Register>(index / REGISTERS_PER_ENCODING, static_cast<codegen::OperandSize>(index % REGISTERS_PER_ENCODING));
        }

        auto byteIt = std::find(codegen::ByteRegisters.begin(), codegen::ByteRegisters.end(), name);
        if (byteIt != codegen::ByteRegisters.end())
        {
            std::size_t index = byteIt - codegen::ByteRegisters.begin();
            return std::make_unique<Register>(index+4, codegen::OperandSize::Byte, true);
        }

        auto extendedIt = std::find(codegen::ExtendedRegisters.begin(), codegen::ExtendedRegisters.end(), name);
        if (extendedIt != codegen::ExtendedRegisters.end())
        {
            std::size_t index = extendedIt - codegen::ExtendedRegisters.begin();
            return std::make_unique<Register>(index / REGISTERS_PER_ENCODING, static_cast<codegen::OperandSize>(index % REGISTERS_PER_ENCODING), false, true);
        }

        return nullptr;
    }

    codegen::REX Register::getRex() const
    {
        codegen::REX rex = codegen::REX::None;

        if (mRex) rex |= codegen::REX::REX;
        if (mSize == codegen::OperandSize::Quad) rex |= codegen::REX::W;

        return rex;
    }

    std::unique_ptr<Operand> Register::clone()
    {
        return std::make_unique<Register>(mID, mSize);
    }

    std::unique_ptr<Register> Register::clone(codegen::OperandSize newSize)
    {
        return std::make_unique<Register>(mID, newSize);
    }
}