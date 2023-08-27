// Copyright 2023 solar-mist


#include "vasm/instruction/operand/Register.h"

namespace instruction
{
    Register::Register(std::uint8_t id, codegen::OperandSize size)
        : mID(id)
        , mSize(size)
    {
    }

    std::uint8_t Register::getID() const
    {
        return mID;
    }

    codegen::OperandSize Register::getSize() const
    {
        return mSize;
    }

    RegisterPtr Register::Get(std::string_view name)
    {
        constexpr int REGISTERS_PER_ENCODING = 4;

        std::uint64_t index;
        for (index = 0; index < static_cast<std::uint64_t>(codegen::Registers.size()); index++)
        {
            if (codegen::Registers[index] == name)
            {
                break;
            }
        }

        return std::make_unique<Register>(index / REGISTERS_PER_ENCODING, static_cast<codegen::OperandSize>(index % REGISTERS_PER_ENCODING));
    }

    std::unique_ptr<Operand> Register::clone()
    {
        return std::make_unique<Register>(mID, mSize);
    }
}