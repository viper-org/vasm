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
        unsigned char byte = 0;
        byte |= (static_cast<unsigned char>(mAddressingMode) << 6);
        byte |= (mReg << 3);
        byte |= mRM;

        outputFormat->write(byte, section);
    }


    SIB::SIB()
        : mScale(0xff)
        , mIndex(0xff)
        , mBase(0xff)
    {
    }
    SIB::SIB(unsigned char scale, Register index, Register base)
        : mScale(scale)
        , mIndex(index)
        , mBase(base)
    {
    }

    void SIB::emit(codegen::IOutputFormat* outputFormat, codegen::Section section)
    {
        unsigned char byte = 0;
        byte |= (static_cast<unsigned char>(mScale) << 6);
        byte |= (mIndex << 3);
        byte |= mBase;

        outputFormat->write(byte, section);
    }
}