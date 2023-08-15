// Copyright 2023 solar-mist


#include "vasm/instruction/operand/String.h"

namespace instruction
{
    String::String(std::string text)
        : mText(text)
    {
    }

    std::string_view String::getText() const
    {
        return mText;
    }

    std::unique_ptr<Operand> String::clone()
    {
        return std::make_unique<String>(mText);
    }
}