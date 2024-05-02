// Copyright 2023 solar-mist


#include "vasm/instruction/Label.h"

namespace instruction
{
    Label::Label(std::string name, bool isGlobal)
        : mName(std::move(name))
        , mIsGlobal(isGlobal)
    {
    }

    void Label::emit(codegen::OpcodeBuilder& builder, codegen::Section section)
    {
        builder.addLabel(mName, section, mIsGlobal);
    }
}