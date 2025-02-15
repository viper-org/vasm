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

    void LabelOperand::reloc(codegen::OpcodeBuilder& builder, codegen::Section section, codegen::OperandSize size, int offset, int addend)
    {
        if (mName != "$")
        {
            if (builder.getLabel(mName).first == -1) // Forward label
            {
                builder.forwardLabel(mName, section, size, offset);
            }
            else
            {
                builder.relocLabel(mName, mLocation, section, offset, addend);
            }
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

    codegen::Section LabelOperand::getSection(codegen::OpcodeBuilder& builder) const
    {
        return builder.getLabelSection(mName);
    }

    std::unique_ptr<Operand> LabelOperand::clone()
    {
        return std::make_unique<LabelOperand>(mName, mLocation);
    }

    std::string LabelOperand::toString()
    {
        return mName;
    }

    std::string_view LabelOperand::getName() const
    {
        return mName;
    }

    std::string_view LabelOperand::getLocation() const
    {
        return mLocation;
    }
}