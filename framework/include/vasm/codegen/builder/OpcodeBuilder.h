// Copyright 2023 solar-mist

#ifndef VASM_BUILDER_BUILDER_H
#define VASM_BUILDER_BUILDER_H 1

#include "vasm/codegen/builder/Instruction.h"

#include <vector>

namespace codegen
{
    class OpcodeBuilder
    {
    public:
        OpcodeBuilder(codegen::IOutputFormat* outputFormat, const std::string& filename);

        void patchForwardLabels();

        Instruction createInstruction(codegen::Section section);

        void addLabel(std::string name, codegen::Section section);
        void relocLabel(std::string name, std::string location, codegen::Section section, int offset = 0);
        void forwardLabel(std::string name, codegen::Section section, codegen::OperandSize size, int offset = 0);

        void addExtern(const std::string& name);

        std::pair<unsigned long long, bool> getLabel(std::string name);
        unsigned long long getPosition(codegen::Section section);
        bool hadErrors() const;

        void reportError(int line, std::string_view message);

    private:
        codegen::IOutputFormat* mOutputFormat;
        std::vector<std::tuple<std::string, codegen::Section, codegen::OperandSize, int, int>> mForwardLabels;
        std::string mFileName;
        bool mHadError; // Stop generating code if an error has been reported
    };
}

#endif