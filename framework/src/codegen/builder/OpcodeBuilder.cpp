// Copyright 2023 solar-mist


#include "codegen/builder/OpcodeBuilder.h"

namespace codegen
{
    OpcodeBuilder::OpcodeBuilder(codegen::IOutputFormat* outputFormat)
        : mOutputFormat(outputFormat)
    {
    }

    Instruction OpcodeBuilder::createInstruction(codegen::Section section)
    {
        return Instruction(mOutputFormat, section);
    }
}