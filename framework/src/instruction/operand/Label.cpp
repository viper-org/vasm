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
        if (mName != "$")
        {
            builder.relocLabel(mName, section, offset);
        }
    }

    std::pair<unsigned long long, bool> LabelOperand::getValue(codegen::OpcodeBuilder& builder, codegen::Section section) const
    {
        if (mName == "$")
        {
            return std::make_pair(builder.getPosition(section), false);
        }
        return builder.getLabel(mName);
    }
}