// Copyright 2023 solar-mist


#include "instruction/Label.h"

namespace instruction
{
    Label::Label(std::string name)
        : mName(name)
    {
    }

    void Label::emit(codegen::OpcodeBuilder& builder, codegen::Section section)
    {
        builder.addLabel(mName, section);
    }
}