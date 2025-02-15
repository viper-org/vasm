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
        Relative(LabelOperandPtr reg, std::optional<int> displacement);

        LabelOperand* getLabel() const;
        std::optional<int> getDisplacement() const;

        std::unique_ptr<Operand> clone() override;

        std::string toString() override;

    private:
        LabelOperandPtr mLabel;
        std::optional<int> mDisplacement;
    };
    using RelativePtr = std::unique_ptr<Relative>;
}

#endif