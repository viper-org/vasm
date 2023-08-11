// Copyright 2023 solar-mist

#include "codegen/Opcodes.h"
#ifndef VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_H 1

#include "instruction/Instruction.h"

namespace instruction
{
    class NoOperandInstruction : public Instruction
    {
    public:
        virtual ~NoOperandInstruction() { }
    };

    template <typename T>
    concept OpcodeT = std::is_same_v<codegen::ByteOpcodes, T> || std::is_same_v<codegen::WordOpcodes, T>;

    template <OpcodeT auto Opcode>
    class NoOperandInstructionTemplate : public NoOperandInstruction
    {
    public:
        NoOperandInstructionTemplate() { }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override
        {
            builder.createInstruction(section)
                   .opcode(Opcode)
                   .emit();
        }
    };

    using RetInstruction     = NoOperandInstructionTemplate<codegen::RET>;
    using SyscallInstruction = NoOperandInstructionTemplate<codegen::SYSCALL>;
}

#endif