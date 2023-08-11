// Copyright 2023 solar-mist


#include "codegen/builder/Instruction.h"

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

    Instruction& Instruction::immediate(unsigned long imm64)
    {
        mImmediate = imm64;
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
            [](std::monostate) {}
        }, mOpcode);

        if (mModRM)
        {
            mModRM->emit(mOutputFormat, mSection);
        }

        std::visit(overloaded {
            [this](auto arg) { mOutputFormat->write(arg, mSection); },
            [](std::monostate) {}
        }, mImmediate);
    }



    Instruction::Instruction(codegen::IOutputFormat* outputFormat, codegen::Section section)
        : mOutputFormat(outputFormat)
        , mSection(section)
    {
    }
}