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

        virtual void write(char  const data, Section const section) = 0;
        virtual void write(short const data, Section const section) = 0;
        virtual void write(int   const data, Section const section) = 0;
        virtual void write(long  const data, Section const section) = 0;

        virtual void print(std::ostream& stream) = 0;
    };
}

#endif