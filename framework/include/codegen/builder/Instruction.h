// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_INSTRUCTION_H
#define VASM_BUILDER_INSTRUCTION_H 1

#include "codegen/builder/ModRM.h"

#include "codegen/IOutputFormat.h"
#include "codegen/Opcodes.h"

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

        Instruction& opcode(codegen::ByteOpcodes byteOpcode);
        Instruction& opcode(codegen::WordOpcodes wordOpcode);

        Instruction& modrm(ModRM modRM);
        Instruction& modrm(AddressingMode addressingMode, ModRM::Register reg, ModRM::Register rm);

        Instruction& immediate(unsigned char  imm8);
        Instruction& immediate(unsigned short imm16);
        Instruction& immediate(unsigned int   imm32);
        Instruction& immediate(unsigned long  imm64);

        Instruction& string(std::string_view str);

        void emit();

    private:
        Instruction(codegen::IOutputFormat* outputFormat, codegen::Section section);

        std::optional<Prefix> mPrefix;
        std::variant<std::monostate, codegen::ByteOpcodes, codegen::WordOpcodes> mOpcode;
        std::optional<ModRM> mModRM;
        std::variant<std::monostate, unsigned char, unsigned short, unsigned int, unsigned long> mImmediate;
        std::optional<std::string_view> mString;

        codegen::IOutputFormat* mOutputFormat;
        codegen::Section mSection;
    };
}

#endif