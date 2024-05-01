// Copyright 2023 solar-mist


#include "vasm/instruction/Directive.h"

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

    SectionDirective::SectionDirective(const std::string& name)
        : mName(name)
    {
    }

    void SectionDirective::emit(codegen::OpcodeBuilder& builder, codegen::Section)
    {
        builder.setSection(builder.getSectionByName(mName));
    }
}