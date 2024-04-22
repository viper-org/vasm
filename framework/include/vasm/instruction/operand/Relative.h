// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_RELATIVE_H
#define VASM_INSTRUCTION_OPERAND_RELATIVE_H 1

#include "vasm/instruction/Operand.h"

#include <optional>

namespace instruction
{
    class LabelOperand;
    using LabelOperandPtr = std::unique_ptr<LabelOperand>;

    class Relative : public Operand
    {
    public:
        Relative(LabelOperandPtr reg);

        LabelOperand* getLabel() const;

        std::unique_ptr<Operand> clone() override;

    private:
        LabelOperandPtr mLabel;
    };
    using RelativePtr = std::unique_ptr<Relative>;
}

#endif