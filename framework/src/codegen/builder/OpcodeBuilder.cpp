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

    void OpcodeBuilder::addExtern(const std::string& name)
    {
        mOutputFormat->addExternSymbol(name);
    }

    std::pair<unsigned long long, bool> OpcodeBuilder::getLabel(std::string name)
    {
        return mOutputFormat->getSymbol(name);
    }

    unsigned long long OpcodeBuilder::getPosition(codegen::Section section)
    {
        return mOutputFormat->getPosition(section);
    }
}