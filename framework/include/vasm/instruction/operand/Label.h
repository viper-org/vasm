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
        codegen::Section getSection(codegen::OpcodeBuilder& builder) const;

        void reloc(codegen::OpcodeBuilder& builder, codegen::Section section, codegen::OperandSize size, int offset = 0, int addend = 0);

        std::unique_ptr<Operand> clone() override;

        std::string toString() override;

        std::string_view getName() const;
        std::string_view getLocation() const;

    private:
        std::string mName;
        std::string mLocation;
    };
    using LabelOperandPtr = std::unique_ptr<LabelOperand>;
}

#endif