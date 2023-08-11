// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_INSTRUCTION_H
#define VASM_INSTRUCTION_INSTRUCTION_H 1

#include "instruction/Value.h"

#include <tuple>
#include <memory>
#include <vector>
#include <type_traits>

namespace instruction
{
    class Instruction : public Value
    {
    public:
        virtual ~Instruction() { }
    };

    using InstructionPtr = std::unique_ptr<Instruction>;
}

#endif