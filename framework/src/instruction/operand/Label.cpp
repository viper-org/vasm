// Copyright 2023 solar-mist


#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    LabelOperand::LabelOperand(std::string name, std::string location)
        : Immediate(0)
        , mName(name)
        , mLocation(location)
    {
    }

    void LabelOperand::reloc(codegen::OpcodeBuilder& builder, codegen::Section section, int offset)
    {
        if (mName != "$")
        {
            builder.relocLabel(mName, mLocation, section, offset);
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