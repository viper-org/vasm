// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_STRING_H
#define VASM_INSTRUCTION_OPERAND_STRING_H 1

#include "instruction/Operand.h"

namespace instruction
{
    class String : public Operand
    {
    public:
        String(std::string text);

        std::string_view getText() const;

    private:
        std::string mText;
    };
    using StringPtr = std::unique_ptr<String>;
}

#endif