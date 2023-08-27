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

        std::pair<std::uint64_t, bool> getValue(codegen::OpcodeBuilder& builder, codegen::Section section) const;

        void reloc(codegen::OpcodeBuilder& builder, codegen::Section section, codegen::OperandSize size, int offset = 0);

        std::unique_ptr<Operand> clone() override;

    private:
        std::string mName;
        std::string mLocation;
    };
    using LabelOperandPtr = std::unique_ptr<LabelOperand>;
}

#endif