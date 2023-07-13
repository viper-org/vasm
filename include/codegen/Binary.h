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

        void write(char  const data, Section const section) override;
        void write(short const data, Section const section) override;
        void write(int   const data, Section const section) override;
        void write(long  const data, Section const section) override;

        int getPosition() override;
        int getSectionStart(Section const section) override;

        void print(std::ostream& stream) override;
    private:
        std::stringstream mBuffer;
    };
}

#endif