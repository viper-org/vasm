// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_LABEL_H
#define VASM_INSTRUCTION_LABEL_H 1

#include "vasm/instruction/Value.h"

#include <memory>

namespace instruction
{
    class Label : public Value
    {
    public:
        Label(std::string name, bool isGlobal);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section);

    private:
        std::string mName;
        bool mIsGlobal;
    };

    using LabelPtr = std::unique_ptr<Label>;
}

#endif