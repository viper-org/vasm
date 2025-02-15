// Copyright 2023 solar-mist


#include "vasm/instruction/operand/String.h"

#include <format>

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

    std::string String::toString()
    {
        std::string copy = mText;
        for (auto it = copy.begin(); it != copy.end(); ++it)
        {
            auto& ch = *it;
            if (ch == '\0')
            {
                ch = '0';
                it = copy.insert(it, '\\') + 1;
            }
            else if (ch == '\n')
            {
                ch = 'n';
                it = copy.insert(it, '\\') + 1;
            }
            else if (ch == '\n')
            {
                ch = 'n';
                it = copy.insert(it, '\\') + 1;
            }
            else if (ch == '\'')
            {
                ch = '\'';
                it = copy.insert(it, '\\') + 1;
            }
        }
        return std::format("\"{}\"", std::move(copy));
    }
}