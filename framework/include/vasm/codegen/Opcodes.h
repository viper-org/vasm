#ifndef VASM_CODEGEN_OPCODES_H
#define VASM_CODEGEN_OPCODES_H 1

#include <array>
#include <string>
#include <string_view>

namespace codegen
{
    enum class REX : unsigned char
    {
        None = 0,

        B = 0b01000001,
        X = 0b01000010,
        R = 0b01000100,
        W = 0b01001000,
        REX = 0b01000000,
    };

    inline constexpr REX& operator|=(REX& a, REX b)
    {
        a = static_cast<REX>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
        return a;
    }

    enum class OperandSize
    {
        Byte = 0,
        Word,
        Long,
        Quad,

        None,
    };

    enum ByteOpcodes : unsigned char
    {
        LOGICAL_REG8_IMM8 = 0x80,
        LOGICAL_REG_IMM = 0x81,
        LOGICAL_REG_IMM8 = 0x83,

        GRP4_RM8 = 0xF6,
        GRP4_RM  = 0xF7,
        

        IMUL_REG_RM_IMM8 = 0x6B,
        IMUL_REG_RM_IMM = 0x69,


        TEST_RM_IMM8 = 0xF6,
        TEST_RM_IMM  = 0xF7,

        TEST_RM_REG8 = 0x84,
        TEST_RM_REG  = 0x85,

        
        SHIFT_ROTATE_RM8_1 = 0xD0,
        SHIFT_ROTATE_RM8_CL = 0xD2,
        SHIFT_ROTATE_RM8_IMM = 0xC0,

        SHIFT_ROTATE_RM_1 = 0xD1,
        SHIFT_ROTATE_RM_CL = 0xD3,
        SHIFT_ROTATE_RM_IMM = 0xC1,


        INC_DEC_REG8 = 0xFE,
        INC_DEC_REG = 0xFF,
        PUSH_RM = 0xFF,


        XCHG_REG_REG8 = 0x86,
        XCHG_REG_REG = 0x87,


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


        LEAVE = 0xC9,


        MOVSXD = 0x63,


        MOV_REG_IMM8 = 0xB0,
        MOV_REG_IMM  = 0xB8,
        MOV_RM_IMM8  = 0xC6,
        MOV_RM_IMM   = 0xC7,

        MOV_RM_REG8 = 0x88,
        MOV_RM_REG  = 0x89,
        MOV_REG_RM8 = 0x8A,
        MOV_REG_RM  = 0x8B,


        NOP = 0x90,
        

        LEA = 0x8D,


        PUSH_REG = 0x50,
        POP_REG = 0x58,
        
        PUSH_IMM = 0x68,
        PUSH_IMM8 = 0x6A,


        IN_AL_IMM8 = 0xE4,
        IN_AX_IMM8 = 0xE5,
        IN_AL_DX = 0xEC,
        IN_AX_DX = 0xED,

        OUT_IMM8_AL = 0xE6,
        OUT_IMM8_AX = 0xE7,
        OUT_DX_AL = 0xEE,
        OUT_DX_AX = 0xEF,


        INT = 0xCD,
    };

    enum WordOpcodes : unsigned char
    {
        SYSCALL = 0x05,

        MOVZX8 = 0xB6,
        MOVZX16 = 0xB7,

        MOVSX8 = 0xBE,
        MOVSX16 = 0xBF,

        IMUL_REG_RM = 0xAF,
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
    constexpr std::array const ByteRegisters = {
        "spl"sv, "bpl"sv, "sil"sv, "dil"sv
    };
    constexpr std::array const ExtendedRegisters = {
        "r8b"sv, "r8w"sv, "r8d"sv, "r8"sv,
        "r9b"sv, "r9w"sv, "r9d"sv, "r9"sv,
        "r10b"sv, "r10w"sv, "r10d"sv, "r10"sv,
        "r11b"sv, "r11w"sv, "r11d"sv, "r11"sv,
        "r12b"sv, "r12w"sv, "r12d"sv, "r12"sv,
        "r13b"sv, "r13w"sv, "r13d"sv, "r13"sv,
        "r14b"sv, "r14w"sv, "r14d"sv, "r14"sv,
        "r15b"sv, "r15w"sv, "r15d"sv, "r15"sv,
    };

    constexpr unsigned char SIZE_PREFIX = 0x66;
}

#endif