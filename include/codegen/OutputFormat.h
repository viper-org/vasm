#ifndef VASM_CODEGEN_OUTPUT_FORMAT_H
#define VASM_CODEGEN_OUTPUT_FORMAT_H 1

#include <sstream>
#include <vector>

namespace Codegen
{
    enum class Section
    {
        Text,
        Data,
    };

    class OutputFormat
    {
    public:
        virtual ~OutputFormat() = default;

        virtual void write(unsigned char  const data, Section const section) = 0;
        virtual void write(unsigned short const data, Section const section) = 0;
        virtual void write(unsigned int   const data, Section const section) = 0;
        virtual void write(unsigned long  const data, Section const section) = 0;

        virtual int getPosition() = 0;
        virtual int getSectionStart(Section const section) = 0;

        virtual void addSymbol(const std::string& name, long long value) = 0;
        virtual long long getSymbol(const std::string& name) = 0;

        virtual void print(std::ostream& stream) = 0;
    };
}

#endif