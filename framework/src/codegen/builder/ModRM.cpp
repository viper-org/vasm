// Copyright 2023 solar-mist


#include "vasm/codegen/builder/ModRM.h"

namespace codegen
{
    ModRM::ModRM(AddressingMode addressingMode, Register reg, Register RM)
        : mAddressingMode(addressingMode)
        , mReg(reg)
        , mRM(RM)
    {
    }

    AddressingMode ModRM::getAddressingMode() const
    {
        return mAddressingMode;
    }

    ModRM::Register ModRM::getReg() const
    {
        return mReg;
    }

    ModRM::Register ModRM::getRM() const
    {
        return mRM;
    }

    void ModRM::emit(codegen::IOutputFormat* outputFormat, codegen::Section section)
    {
        std::uint8_t byte = 0;
        byte |= (static_cast<std::uint8_t>(mAddressingMode) << 6);
        byte |= (mReg << 3);
        byte |= mRM;

        outputFormat->write(byte, section);
    }
}