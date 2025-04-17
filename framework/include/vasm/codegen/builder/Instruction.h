// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_INSTRUCTION_H
#define VASM_BUILDER_INSTRUCTION_H 1

#include "vasm/codegen/builder/ModRM.h"

#include "vasm/codegen/IOutputFormat.h"
#include "vasm/codegen/Opcodes.h"

#include <cstdint>
#include <optional>
#include <variant>

namespace codegen
{
    class OpcodeBuilder;

    class Instruction
    {
    friend class OpcodeBuilder;
    using Prefix = unsigned char;
    public:

        Instruction& prefix(Prefix newPrefix);
        Instruction& prefix(codegen::REX rex);

        Instruction& opcode(codegen::ByteOpcodes byteOpcode);
        Instruction& opcode(codegen::WordOpcodes wordOpcode);

        Instruction& modrm(ModRM modRM);
        Instruction& modrm(AddressingMode addressingMode, ModRM::Register reg, ModRM::Register rm);
        Instruction& sib(SIB sib);
        Instruction& sib(std::optional<unsigned char> scale, SIB::Register index, SIB::Register base);

        Instruction& displacement(std::optional<int> disp, bool sizeOverride = false);

        Instruction& immediate(std::uint8_t  imm8);
        Instruction& immediate(std::uint16_t imm16);
        Instruction& immediate(std::uint32_t imm32);
        Instruction& immediate(std::uint64_t imm64);

        Instruction& string(std::string_view str, bool nullterm = false);

        void emit(std::uint64_t offset = -1);

    private:
        Instruction(codegen::IOutputFormat* outputFormat, codegen::Section section);

        std::optional<Prefix> mPrefix;
        std::optional<Prefix> mRex;
        std::variant<std::monostate, codegen::ByteOpcodes, codegen::WordOpcodes> mOpcode;
        std::optional<ModRM> mModRM;
        std::optional<SIB> mSIB;
        std::optional<int> mDisplacement;
        bool mDisplacementSizeOverride;
        std::variant<std::monostate, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t> mImmediate;
        std::optional<std::string_view> mString;
        bool mNullTerm;

        codegen::IOutputFormat* mOutputFormat;
        codegen::Section mSection;
    };
}

#endif