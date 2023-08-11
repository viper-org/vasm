// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_LABEL_H
#define VASM_INSTRUCTION_OPERAND_LABEL_H 1

#include "instruction/operand/Immediate.h"

#include "codegen/builder/OpcodeBuilder.h"

namespace instruction
{
    class LabelOperand : public Immediate
    {
    public:
        LabelOperand(std::string name);

        unsigned long long getValue(codegen::OpcodeBuilder& builder) const override;

        void reloc(codegen::OpcodeBuilder& builder, codegen::Section section, int offset = 0);

    private:
        std::string mName;
    };
    using LabelOperandPtr = std::unique_ptr<LabelOperand>;
}

#endif