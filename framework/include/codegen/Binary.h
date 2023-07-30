#ifndef VASM_CODEGEN_BINARY_H
#define VASM_CODEGEN_BINARY_H 1

#include "codegen/OutputFormat.h"

#include <unordered_map>
#include <vector>

namespace Lexing
{
    class Token;
}

namespace Codegen
{
    class BinaryFormat : public OutputFormat
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
        [[nodiscard]] unsigned long getSymbol(const std::string& name) const override;
        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, Section section) override;

        void print(std::ostream& stream) override;
    private:
        std::vector<unsigned char> mBuffer;
        std::unordered_map<std::string, unsigned long> mSymbols;
    };
}

#endif