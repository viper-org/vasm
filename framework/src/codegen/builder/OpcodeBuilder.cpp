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

    void OpcodeBuilder::addLabel(std::string name, codegen::Section section)
    {
        mOutputFormat->addSymbol(name, mOutputFormat->getPosition(section), section, true);
    }

    void OpcodeBuilder::relocLabel(std::string name, codegen::Section section, int offset)
    {
        mOutputFormat->relocSymbol(name, section, offset);
    }

    unsigned long long OpcodeBuilder::getLabel(std::string name)
    {
        return mOutputFormat->getSymbol(name);
    }
}