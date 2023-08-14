// Copyright 2023 solar-mist


#include "instruction/Directive.h"

namespace instruction
{
    ExternDirective::ExternDirective(const std::string& name)
        : mName(name)
    {
    }

    void ExternDirective::emit(codegen::OpcodeBuilder& builder, codegen::Section)
    {
        builder.addExtern(mName);
    }
}