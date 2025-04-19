// Copyright 2023 solar-mist


#include "vasm/codegen/builder/Instruction.h"

#include <cstdint>
#include <iostream>

namespace codegen
{
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    Instruction& Instruction::prefix(Prefix newPrefix)
    {
        if (!mPrefix)
            mPrefix = newPrefix;
        else
            *mPrefix |= newPrefix;
        return *this;
    }

    Instruction& Instruction::prefix(codegen::REX rex)
    {
        if (!mRex)
            mRex = static_cast<Prefix>(rex);
        else
            *mRex |= static_cast<Prefix>(rex);
        return *this;
    }


    Instruction& Instruction::opcode(codegen::ByteOpcodes opcode)
    {
        mOpcode = opcode;
        return *this;
    }

    Instruction& Instruction::opcode(codegen::WordOpcodes opcode)
    {
        mOpcode = opcode;
        return *this;
    }


    Instruction& Instruction::modrm(ModRM modRM)
    {
        mModRM = modRM;
        return *this;
    }

    Instruction& Instruction::modrm(AddressingMode addressingMode, ModRM::Register reg, ModRM::Register rm)
    {
        mModRM = ModRM(addressingMode, reg, rm);
        return *this;
    }

    Instruction& Instruction::sib(SIB sib)
    {
        if (sib.mScale != 0xff)
        {
            mSIB = sib;
            mModRM->mRM = 0b100;
        }
        return *this;
    }

    Instruction& Instruction::sib(std::optional<unsigned char> scale, SIB::Register index, SIB::Register base)
    {
        if (scale)
        {
            mSIB = SIB(scale?*scale:0, index, base);
            mModRM->mRM = 0b100;
        }
        return *this;
    }


    Instruction& Instruction::displacement(std::optional<int> disp, bool sizeOverride)
    {
        mDisplacement = disp;
        mDisplacementSizeOverride = sizeOverride;
        return *this;
    }


    Instruction& Instruction::immediate(std::uint8_t imm8)
    {
        mImmediate = imm8;
        return *this;
    }

    Instruction& Instruction::immediate(std::uint16_t imm16)
    {
        mImmediate = imm16;
        return *this;
    }

    Instruction& Instruction::immediate(std::uint32_t imm32)
    {
        mImmediate = imm32;
        return *this;
    }

    Instruction& Instruction::immediate(std::uint64_t imm64)
    {
        mImmediate = imm64;
        return *this;
    }


    Instruction& Instruction::string(std::string_view str, bool nullterm)
    {
        mString = str;
        mNullTerm = nullterm;
        return *this;
    }


    void Instruction::emit(std::uint64_t offset, bool overwrite)
    {
        if (mPrefix && *mPrefix != 0)
        {
            mOutputFormat->write(*mPrefix, mSection);
        }
        if (mRex && *mRex != 0)
        {
            mOutputFormat->write(*mRex, mSection);
        }

        std::visit(overloaded {
            [this](auto arg) { mOutputFormat->write(arg, mSection); },
            [this](codegen::WordOpcodes arg) { 
                mOutputFormat->write(static_cast<unsigned char>(0x0F), mSection);
                mOutputFormat->write(arg, mSection);
            },
            [](std::monostate) {}
        }, mOpcode);

        if (mModRM)
        {
            mModRM->emit(mOutputFormat, mSection);
        }
        if (mSIB)
        {
            mSIB->emit(mOutputFormat, mSection);
        }

        if (mDisplacement)
        {
            if (*mDisplacement <= INT8_MAX && !mDisplacementSizeOverride)
            {
                mOutputFormat->write(static_cast<unsigned char>(*mDisplacement), mSection);
            }
            else
            {
                mOutputFormat->write(static_cast<unsigned int>(*mDisplacement), mSection);
            }
        }

        if (mImmediate.index() != 0)
        {
            std::visit(overloaded {
                [this, offset, overwrite](auto arg) { mOutputFormat->write(arg, mSection, offset, overwrite); },
                [](std::monostate) {}
            }, mImmediate);
        }

        if (mString)
        {
            for (unsigned char ch : *mString)
            {
                mOutputFormat->write(ch, mSection);
            }
            if (mNullTerm)
            {
                mOutputFormat->write((uint8_t)0, mSection);
            }
        }
    }



    Instruction::Instruction(codegen::IOutputFormat* outputFormat, codegen::Section section)
        : mOutputFormat(outputFormat)
        , mSection(section)
    {
    }
}