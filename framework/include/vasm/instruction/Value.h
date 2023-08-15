// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_VALUE_H
#define VASM_INSTRUCTION_VALUE_H 1

#include "vasm/codegen/builder/OpcodeBuilder.h"

#include <memory>

namespace instruction
{
    class Value
    {
    public:
        virtual ~Value() { }

        virtual void emit(codegen::OpcodeBuilder& builder, codegen::Section section) = 0;
    };

    using ValuePtr = std::unique_ptr<Value>;
}

#endif