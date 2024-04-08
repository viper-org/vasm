// Copyright 2023 solar-mist


#include "vasm/codegen/builder/OpcodeBuilder.h"

#include <format>
#include <iostream>

namespace codegen
{
    OpcodeBuilder::OpcodeBuilder(codegen::IOutputFormat* outputFormat, const std::string& filename)
        : mOutputFormat(outputFormat)
        , mFileName(filename)
    {
    }

    void OpcodeBuilder::patchForwardLabels()
    {
        for (auto label : mForwardLabels)
        {
            mOutputFormat->patchForwardSymbol(std::get<0>(label), std::get<1>(label), std::get<2>(label), std::get<3>(label), std::get<4>(label));
        }
    }

    Instruction OpcodeBuilder::createInstruction(codegen::Section section)
    {
        return Instruction(mOutputFormat, section);
    }

    void OpcodeBuilder::addLabel(std::string name, codegen::Section section)
    {
        mOutputFormat->addSymbol(name, mOutputFormat->getPosition(section), section, true);
    }

    void OpcodeBuilder::relocLabel(std::string name, std::string location, codegen::Section section, int offset)
    {
        mOutputFormat->relocSymbol(name, location, section, offset);
    }

    void OpcodeBuilder::forwardLabel(std::string name, codegen::Section section, codegen::OperandSize size, int offset)
    {
        mForwardLabels.emplace_back(name, section, size, mOutputFormat->getPosition(section) + offset, mOutputFormat->getPosition(section));
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

    bool OpcodeBuilder::hadErrors() const
    {
        return mHadError;
    }


    void OpcodeBuilder::reportError(int line, std::string_view message)
    {
        std::cerr << std::format("{}:{}: error: {}\n", mFileName, line, message);
        mHadError = true;
    }
}