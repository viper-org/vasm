// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_BUILDER_H
#define VASM_BUILDER_BUILDER_H 1

#include "vasm/codegen/builder/Instruction.h"

#include <cstdint>
#include <vector>

namespace codegen
{
    class OpcodeBuilder
    {
    public:
        OpcodeBuilder() = default;
        OpcodeBuilder(codegen::IOutputFormat* outputFormat, const std::string& filename);

        void patchForwardLabels();

        Instruction createInstruction(codegen::Section section);

        void addLabel(std::string name, codegen::Section section, bool isGlobal);
        void relocLabel(std::string name, std::string location, codegen::Section section, int offset = 0, int addend = 0);
        void forwardLabel(std::string name, codegen::Section section, codegen::OperandSize size, int offset = 0);

        void addExtern(const std::string& name);

        std::pair<std::uint64_t, bool> getLabel(std::string name);
        std::string getLabelAfter(std::string name);
        Section getLabelSection(std::string_view name);
        std::uint64_t getPosition(codegen::Section section);
        bool hadErrors() const;

        codegen::Section getSectionByName(std::string_view name);
        codegen::Section getSection() const;
        void setSection(codegen::Section);

        void reportError(int line, std::string_view message);

    private:
        codegen::IOutputFormat* mOutputFormat;
        std::vector<std::tuple<std::string, codegen::Section, codegen::OperandSize, int, int>> mForwardLabels;
        std::string mFileName;
        codegen::Section mSection;
        bool mHadError; // Stop generating code if an error has been reported
    };
}

#endif