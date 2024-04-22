// Copyright 2024 solar-mist


#include "vasm/instruction/operand/Relative.h"
#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    Relative::Relative(LabelOperandPtr label)
        : mLabel(std::move(label))
    {
    }

    LabelOperand* Relative::getLabel() const
    {
        return mLabel.get();
    }

    std::unique_ptr<Operand> Relative::clone()
    {
        return std::make_unique<Relative>(LabelOperandPtr(static_cast<LabelOperand*>(mLabel->clone().release())));
    }
}