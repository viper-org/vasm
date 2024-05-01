// Copyright 2024 solar-mist


#include "vasm/instruction/operand/Relative.h"
#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    Relative::Relative(LabelOperandPtr label, std::optional<int> displacement)
        : mLabel(std::move(label))
        , mDisplacement(displacement)
    {
    }

    LabelOperand* Relative::getLabel() const
    {
        return mLabel.get();
    }

    std::optional<int> Relative::getDisplacement() const
    {
        return mDisplacement;
    }

    std::unique_ptr<Operand> Relative::clone()
    {
        return std::make_unique<Relative>(LabelOperandPtr(static_cast<LabelOperand*>(mLabel->clone().release())), mDisplacement);
    }
}