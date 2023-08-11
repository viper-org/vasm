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

    private:
        codegen::IOutputFormat* mOutputFormat;
    };
}

#endif