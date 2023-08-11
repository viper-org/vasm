// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_INSTRUCTION_H
#define VASM_INSTRUCTION_INSTRUCTION_H 1

#include "codegen/builder/OpcodeBuilder.h"

#include <memory>

namespace instruction
{
    class Instruction
    {
    public:
        virtual ~Instruction() { }

        virtual void emit(codegen::OpcodeBuilder& builder, codegen::Section section) const = 0;
    };

    using InstructionPtr = std::unique_ptr<Instruction>;
}

#endif