#ifndef VASM_CODEGEN_OPCODES_H
#define VASM_CODEGEN_OPCODES_H 1

#include <array>
#include <string_view>

namespace Codegen
{
    enum REX : unsigned char
    {
        B = 0b01000001,
        X = 0b01000010,
        R = 0b01000100,
        W = 0b01001000,
    };

    enum class OperandSize
    {
        Byte = 0,
        Word,
        Long,
        Quad,
    };

    enum Opcodes : unsigned char
    {
        JMP_REL8 = 0xEB,
        RET = 0xC3,

        MOV_REG_IMM8 = 0xB0,
        MOV_REG_IMM  = 0xB8,

        MOV_REG_REG8 = 0x88,
        MOV_REG_REG  = 0x89,

        INT = 0xCD,
    };

    using namespace std::literals;
    constexpr std::array const Registers = {
        "al"sv, "ax"sv, "eax"sv, "rax"sv,
        "cl"sv, "cx"sv, "ecx"sv, "rcx"sv,
        "dl"sv, "dx"sv, "edx"sv, "rdx"sv,
        "bl"sv, "bx"sv, "ebx"sv, "rbx"sv,
        "ah"sv, "sp"sv, "esp"sv, "rsp"sv,
        "ch"sv, "bp"sv, "ebp"sv, "rbp"sv,
        "dh"sv, "si"sv, "esi"sv, "rsi"sv,
        "bh"sv, "di"sv, "edi"sv, "rdi"sv,
    };
}

#endif