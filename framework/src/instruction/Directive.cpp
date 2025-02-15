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

    void ExternDirective::print(std::ostream& stream)
    {
        stream << "extern " << mName;
    }

    SectionDirective::SectionDirective(const std::string& name)
        : mName(name)
    {
    }

    void SectionDirective::emit(codegen::OpcodeBuilder& builder, codegen::Section)
    {
        builder.setSection(builder.getSectionByName(mName));
    }

    void SectionDirective::print(std::ostream& stream)
    {
        stream << "section " << mName;
    }
}