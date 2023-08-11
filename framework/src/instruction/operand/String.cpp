// Copyright 2023 solar-mist


#include "instruction/operand/String.h"

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
}