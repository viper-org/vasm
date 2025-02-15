// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_INSTRUCTION_H
#define VASM_INSTRUCTION_INSTRUCTION_H 1

#include "vasm/instruction/Value.h"

#include <tuple>
#include <memory>
#include <vector>
#include <type_traits>

namespace instruction
{
    template <size_t N>
    struct InstructionStringBuilder
    {
        consteval InstructionStringBuilder(const char(&str)[N])
        {
            std::copy(str, str + N, mValue);
        }
        consteval operator const char*() { return mValue; }
        char mValue[N];
    };

    template <typename T>
    concept OpcodeT = std::is_same_v<codegen::ByteOpcodes, T> || std::is_same_v<codegen::WordOpcodes, T>;

    class Instruction : public Value
    {
    public:
        virtual ~Instruction() { }
    };

    using InstructionPtr = std::unique_ptr<Instruction>;
}

#endif