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
        mPrefix = newPrefix;
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


    Instruction& Instruction::displacement(std::optional<int> disp)
    {
        mDisplacement = disp;
        return *this;
    }


    Instruction& Instruction::immediate(unsigned char imm8)
    {
        mImmediate = imm8;
        return *this;
    }

    Instruction& Instruction::immediate(unsigned short imm16)
    {
        mImmediate = imm16;
        return *this;
    }

    Instruction& Instruction::immediate(unsigned int imm32)
    {
        mImmediate = imm32;
        return *this;
    }

    Instruction& Instruction::immediate(unsigned long long imm64)
    {
        mImmediate = imm64;
        return *this;
    }


    Instruction& Instruction::string(std::string_view str)
    {
        mString = str;
        return *this;
    }


    void Instruction::emit()
    {
        
        if (mPrefix)
        {
            mOutputFormat->write(*mPrefix, mSection);
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

        if (mDisplacement)
        {
            if (*mDisplacement <= INT8_MAX)
            {
                mOutputFormat->write(static_cast<unsigned char>(*mDisplacement), mSection);
            }
            else
            {
                mOutputFormat->write(static_cast<unsigned int>(*mDisplacement), mSection);
            }
        }

        std::visit(overloaded {
            [this](auto arg) { mOutputFormat->write(arg, mSection); },
            [](std::monostate) {}
        }, mImmediate);

        if (mString)
        {
            for (unsigned char ch : *mString)
            {
                mOutputFormat->write(ch, mSection);
            }
        }
    }



    Instruction::Instruction(codegen::IOutputFormat* outputFormat, codegen::Section section)
        : mOutputFormat(outputFormat)
        , mSection(section)
    {
    }
}