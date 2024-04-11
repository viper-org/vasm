// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_H
#define VASM_INSTRUCTION_NO_OPERAND_INSTRUCTION_H 1

#include "vasm/instruction/Instruction.h"

#include "vasm/codegen/Opcodes.h"

namespace instruction
{
    class NoOperandInstruction : public Instruction
    {
    public:
        NoOperandInstruction(int lineNumber) : mLineNumber(lineNumber) { }
        
        virtual ~NoOperandInstruction() { }

        int getLineNumber() const { return mLineNumber; }

    private:
        int mLineNumber;
    };

    template <OpcodeT auto Opcode>
    class NoOperandInstructionTemplate : public NoOperandInstruction
    {
    public:
        NoOperandInstructionTemplate(int lineNumber=-1) : NoOperandInstruction(lineNumber) { }

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override
        {
            builder.createInstruction(section)
                   .opcode(Opcode)
                   .emit();
        }
    };

    using RetInstruction     = NoOperandInstructionTemplate<codegen::RET>;
    using LeaveInstruction   = NoOperandInstructionTemplate<codegen::LEAVE>;
    using SyscallInstruction = NoOperandInstructionTemplate<codegen::SYSCALL>;
    using NopInstruction     = NoOperandInstructionTemplate<codegen::NOP>;
}

#endif