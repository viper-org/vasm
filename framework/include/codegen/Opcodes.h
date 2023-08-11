#ifndef VASM_CODEGEN_OPCODES_H
#define VASM_CODEGEN_OPCODES_H 1

#include <array>
#include <string_view>

namespace codegen
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

    enum ByteOpcodes : unsigned char
    {
        LOGICAL_REG8_IMM8 = 0x80,
        LOGICAL_REG_IMM = 0x81,
        LOGICAL_REG_IMM8 = 0x83,


        INC_DEC_REG8 = 0xFE,
        INC_DEC_REG = 0xFF,


        JMP_REL8 = 0xEB,

        JA_REL8 = 0x77,
        JAE_REL8 = 0x73,
        JB_REL8 = 0x72,
        JBE_REL8 = 0x76,
        JCXZ_REL8 = 0xE3,
        JECXZ_REL8 = 0xE3,
        JRCXZ_REL8 = 0xE3,
        JE_REL8 = 0x74,
        JG_REL8 = 0x7F,
        JGE_REL8 = 0x7D,
        JL_REL8 = 0x7C,
        JLE_REL8 = 0x7E,
        JNA_REL8 = 0x76,
        JNAE_REL8 = 0x72,
        JNB_REL8 = 0x73,
        JNBE_REL8 = 0x77,
        JNC_REL8 = 0x73,
        JNE_REL8 = 0x75,
        JNG_REL8 = 0x7E,
        JNGE_REL8 = 0x7C,
        JNL_REL8 = 0x7D,
        JNLE_REL8 = 0x7F,
        JNO_REL8 = 0x71,
        JNP_REL8 = 0x7B,
        JNS_REL8 = 0x79,
        JNZ_REL8 = 0x75,
        JO_REL8 = 0x70,
        JP_REL8 = 0x7A,
        JPE_REL8 = 0x7A,
        JPO_REL8 = 0x7B,
        JS_REL8 = 0x78,
        JZ_REL8 = 0x74,


        CALL_REL32 = 0xE8,
        RET = 0xC3,


        MOV_REG_IMM8 = 0xB0,
        MOV_REG_IMM  = 0xB8,

        MOV_REG_REG8 = 0x88,
        MOV_REG_REG  = 0x89,


        NOP = 0x90,
        

        LEA = 0x8D,


        PUSH_REG = 0x50,
        POP_REG = 0x58,
        
        PUSH_IMM = 0x68,
        PUSH_IMM8 = 0x6A,


        INT = 0xCD,
    };

    enum WordOpcodes : unsigned char
    {
        SYSCALL = 0x05,
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

    constexpr unsigned char SIZE_PREFIX = 0x66;
}

#endif