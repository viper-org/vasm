// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_MODRM_H
#define VASM_BUILDER_MODRM_H 1

#include "vasm/codegen/IOutputFormat.h"

namespace codegen
{
    enum class AddressingMode : unsigned char
    {
        RegisterIndirect = 0b00,
        OneByteDisp = 0b01,
        FourByteDisp = 0b10,
        RegisterDirect = 0b11,
    };

    class Instruction;

    class ModRM
    {
    friend class Instruction;
    using Register = unsigned char;
    public:
        ModRM(AddressingMode addressingMode, Register reg, Register RM);

        AddressingMode getAddressingMode() const;
        Register getReg() const;
        Register getRM() const;

        static constexpr Register NullRegister = 0;

    private:
        void emit(codegen::IOutputFormat* outputFormat, std::string section);

        AddressingMode mAddressingMode;

        Register mReg;
        Register mRM;
    };

    class SIB
    {
    friend class Instruction;
    using Register = unsigned char;
    public:
        SIB();
        SIB(unsigned char scale, Register index, Register base);

    private:
        void emit(codegen::IOutputFormat* outputFormat, std::string section);

        unsigned char mScale;
        Register mIndex;
        Register mBase;
    };
}

#endif