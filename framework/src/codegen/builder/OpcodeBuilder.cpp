// Copyright 2023 solar-mist


#include "vasm/codegen/builder/OpcodeBuilder.h"

#include <format>
#include <iostream>

namespace codegen
{
    OpcodeBuilder::OpcodeBuilder(codegen::IOutputFormat* outputFormat, const std::string& filename)
        : mOutputFormat(outputFormat)
        , mFileName(filename)
        , mSection(codegen::Section::Text)
        , mHadError(false)
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

    void OpcodeBuilder::addLabel(std::string name, codegen::Section section, bool isGlobal)
    {
        mOutputFormat->addSymbol(name, mOutputFormat->getPosition(section), section, isGlobal);
    }

    void OpcodeBuilder::relocLabel(std::string name, std::string location, codegen::Section section, int offset, int addend)
    {
        mOutputFormat->relocSymbol(name, location, section, offset, addend);
    }

    void OpcodeBuilder::forwardLabel(std::string name, codegen::Section section, codegen::OperandSize size, int offset)
    {
        mForwardLabels.emplace_back(name, section, size, mOutputFormat->getPosition(section) + offset, mOutputFormat->getPosition(section));
    }

    void OpcodeBuilder::addExtern(const std::string& name)
    {
        mOutputFormat->addExternSymbol(name);
    }

    std::pair<std::uint64_t, bool> OpcodeBuilder::getLabel(std::string name)
    {
        return mOutputFormat->getSymbol(name);
    }

    Section OpcodeBuilder::getLabelSection(std::string_view name)
    {
        return mOutputFormat->getSymbolSection(name);
    }

    std::uint64_t OpcodeBuilder::getPosition(codegen::Section section)
    {
        return mOutputFormat->getPosition(section);
    }

    bool OpcodeBuilder::hadErrors() const
    {
        return mHadError;
    }

    codegen::Section OpcodeBuilder::getSectionByName(std::string_view name)
    {
        return mOutputFormat->getSection(name);
    }

    codegen::Section OpcodeBuilder::getSection() const
    {
        return mSection;
    }
    void OpcodeBuilder::setSection(codegen::Section section)
    {
        mSection = section;
    }


    void OpcodeBuilder::reportError(int line, std::string_view message)
    {
        std::cerr << std::format("{}:{}: error: {}\n", mFileName, line, message);
        mHadError = true;
    }
}