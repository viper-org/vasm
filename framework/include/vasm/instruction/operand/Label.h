// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_LABEL_H
#define VASM_INSTRUCTION_OPERAND_LABEL_H 1

#include "vasm/instruction/operand/Immediate.h"

#include "vasm/codegen/builder/OpcodeBuilder.h"

namespace instruction
{
    class LabelOperand : public Immediate
    {
    public:
        LabelOperand(std::string name, std::string location = "");

        std::pair<unsigned long long, bool> getValue(codegen::OpcodeBuilder& builder, codegen::Section section) const;

        void reloc(codegen::OpcodeBuilder& builder, codegen::Section section, int offset = 0);

    private:
        std::string mName;
        std::string mLocation;
    };
    using LabelOperandPtr = std::unique_ptr<LabelOperand>;
}

#endif