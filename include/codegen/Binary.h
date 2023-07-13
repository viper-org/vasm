#include <unordered_map>
#ifndef VASM_CODEGEN_BINARY_H
#define VASM_CODEGEN_BINARY_H 1

#include <codegen/OutputFormat.h>

#include <sstream>
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

        void write(unsigned char  const data, Section const section) override;
        void write(unsigned short const data, Section const section) override;
        void write(unsigned int   const data, Section const section) override;
        void write(unsigned long  const data, Section const section) override;

        int getPosition() override;
        int getSectionStart(Section const section) override;

        void addSymbol(const std::string& name, long long value) override;
        long long getSymbol(const std::string& name) override;

        void print(std::ostream& stream) override;
    private:
        std::stringstream mBuffer;
        std::unordered_map<std::string, long long> mSymbols;
    };
}

#endif