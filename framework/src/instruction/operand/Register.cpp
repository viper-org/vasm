// Copyright 2023 solar-mist


#include "instruction/operand/Register.h"

namespace instruction
{
    Register::Register(unsigned char id, codegen::OperandSize size)
        : mID(id)
        , mSize(size)
    {
    }

    unsigned char Register::getID() const
    {
        return mID;
    }
}