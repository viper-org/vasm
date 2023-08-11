// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_BUILDER_H
#define VASM_BUILDER_BUILDER_H 1

#include "codegen/builder/Instruction.h"

namespace codegen
{
    class OpcodeBuilder
    {
    public:
        OpcodeBuilder(codegen::IOutputFormat* outputFormat);

        Instruction createInstruction(codegen::Section section);

        void addLabel(std::string name, codegen::Section section);
        void relocLabel(std::string name, codegen::Section section, int offset = 0);

        unsigned long long getLabel(std::string name);
        unsigned long long getPosition(codegen::Section section);

    private:
        codegen::IOutputFormat* mOutputFormat;
    };
}

#endif