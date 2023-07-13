#ifndef VASM_CODEGEN_OUTPUT_FORMAT_H
#define VASM_CODEGEN_OUTPUT_FORMAT_H 1

#include <sstream>
#include <vector>

namespace Codegen
{
    enum REX : char
    {
        B = 0b0100 | 1 << 0,
        X = 0b0100 | 1 << 1,
        R = 0b0100 | 1 << 2,
        W = 0b0100 | 1 << 3,
    };

    enum class Section
    {
        Text,
        Data,
    };

    class OutputFormat
    {
    public:
        virtual ~OutputFormat() = default;

        virtual void write(char  const data, Section const section) = 0;
        virtual void write(short const data, Section const section) = 0;
        virtual void write(int   const data, Section const section) = 0;
        virtual void write(long  const data, Section const section) = 0;

        virtual int getPosition() = 0;
        virtual int getSectionStart(Section const section) = 0;

        virtual void print(std::ostream& stream) = 0;
    };
}

#endif