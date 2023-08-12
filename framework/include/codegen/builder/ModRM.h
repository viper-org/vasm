// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_MODRM_H
#define VASM_BUILDER_MODRM_H 1

#include "codegen/IOutputFormat.h"

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
        void emit(codegen::IOutputFormat* outputFormat, codegen::Section section);

        AddressingMode mAddressingMode;

        Register mReg;
        Register mRM;
    };
}

#endif