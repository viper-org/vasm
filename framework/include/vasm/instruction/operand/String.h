// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_STRING_H
#define VASM_INSTRUCTION_OPERAND_STRING_H 1

#include "vasm/instruction/Operand.h"

#include <string>

namespace instruction
{
    class String : public Operand
    {
    public:
        String(std::string text);

        std::string_view getText() const;

        std::unique_ptr<Operand> clone() override;

    private:
        std::string mText;
    };
    using StringPtr = std::unique_ptr<String>;
}

#endif