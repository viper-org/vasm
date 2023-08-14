#ifndef VASM_CODEGEN_OUTPUT_FORMAT_H
#define VASM_CODEGEN_OUTPUT_FORMAT_H 1

#include <string>
#include <ostream>

namespace codegen
{
    using Global = bool;

    enum class Section
    {
        Text,
        Data,
        Other,
    };

    class IOutputFormat
    {
    public:
        virtual ~IOutputFormat() = default;

        virtual void write(unsigned char  data, Section section) = 0;
        virtual void write(unsigned short data, Section section) = 0;
        virtual void write(unsigned int   data, Section section) = 0;
        virtual void write(unsigned long  data, Section section) = 0;

        virtual size_t getPosition(Section section) = 0;
        virtual size_t getSectionStart(Section section) = 0;

        virtual void addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal) = 0;
        virtual void addExternSymbol(const std::string& name) = 0;
        [[nodiscard]] virtual bool hasSymbol(const std::string& name) const = 0;
        [[nodiscard]] virtual std::pair<unsigned long, bool> getSymbol(const std::string& name) const = 0;
        virtual void relocSymbol(const std::string& name, Section section, int offset = 0) = 0;

        virtual void print(std::ostream& stream) = 0;
    };
}

#endif