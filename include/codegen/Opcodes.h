#ifndef VASM_CODEGEN_OPCODES_H
#define VASM_CODEGEN_OPCODES_H 1

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

        MOV_REG_IMM8 = 0xB0,
        MOV_REG_IMM  = 0xB8,

        MOV_REG_REG8 = 0x88,
        MOV_REG_REG  = 0x89,

        INT = 0xCD,
    };
}

#endif  