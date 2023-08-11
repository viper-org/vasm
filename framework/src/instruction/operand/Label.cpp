// Copyright 2023 solar-mist


#include "instruction/operand/Label.h"

namespace instruction
{
    LabelOperand::LabelOperand(std::string name)
        : Immediate(0)
        , mName(name)
    {
    }

    void LabelOperand::reloc(codegen::OpcodeBuilder& builder, codegen::Section section, int offset)
    {
        builder.relocLabel(mName, section, offset);
    }

    unsigned long long LabelOperand::getValue(codegen::OpcodeBuilder& builder) const
    {
        return builder.getLabel(mName);
    }
}