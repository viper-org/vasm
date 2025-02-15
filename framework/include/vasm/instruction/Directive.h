// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_DIRECTIVE_H
#define VASM_INSTRUCTION_DIRECTIVE_H 1

#include "vasm/instruction/Value.h"

#include <memory>

namespace instruction
{
    class ExternDirective : public Value
    {
    public:
        ExternDirective(const std::string& name);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override;
        
        void print(std::ostream& stream) override;

    private:
        std::string mName;
    };

    class SectionDirective : public Value
    {
    public:
        SectionDirective(const std::string& name);

        void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override;
        
        void print(std::ostream& stream) override;

    private:
        std::string mName;
    };
}

#endif