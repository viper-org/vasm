#ifndef VASM_CODEGEN_BINARY_H
#define VASM_CODEGEN_BINARY_H 1

#include "codegen/IOutputFormat.h"

#include <unordered_map>
#include <vector>

namespace lexing
{
    class Token;
}

namespace codegen
{
    class BinaryFormat : public IOutputFormat
    {
    public:
        BinaryFormat();

        void write(unsigned char  data, Section section) override;
        void write(unsigned short data, Section section) override;
        void write(unsigned int   data, Section section) override;
        void write(unsigned long  data, Section section) override;

        size_t getPosition(Section section) override;
        size_t getSectionStart(Section section) override;

        void addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal) override;
        void addExternSymbol(const std::string& name) override;
        [[nodiscard]] std::pair<unsigned long, bool> getSymbol(const std::string& name) const override;
        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, const std::string& location, Section section, int offset) override;

        void print(std::ostream& stream) override;
    private:
        std::vector<unsigned char> mBuffer;
        std::unordered_map<std::string, unsigned long> mSymbols;
    };
}

#endif