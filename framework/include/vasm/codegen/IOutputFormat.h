#ifndef VASM_CODEGEN_OUTPUT_FORMAT_H
#define VASM_CODEGEN_OUTPUT_FORMAT_H 1

#include "vasm/codegen/Opcodes.h"

#include <cstdint>
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

        virtual void write(std::uint8_t  data, Section section) = 0;
        virtual void write(std::uint16_t data, Section section) = 0;
        virtual void write(std::uint32_t data, Section section) = 0;
        virtual void write(std::uint64_t data, Section section) = 0;

        virtual size_t getPosition(Section section) = 0;
        virtual size_t getSectionStart(Section section) = 0;

        virtual void addSymbol(const std::string& name, std::uint64_t value, Section section, bool isGlobal) = 0;
        virtual void addExternSymbol(const std::string& name) = 0;
        [[nodiscard]] virtual bool hasSymbol(const std::string& name) const = 0;
        [[nodiscard]] virtual std::pair<std::uint64_t, bool> getSymbol(const std::string& name) const = 0;
        virtual void relocSymbol(const std::string& name, const std::string& location, Section section, int offset = 0) = 0;
        virtual void patchForwardSymbol(const std::string& name, Section section, OperandSize size, int location, int origin) = 0;

        virtual void print(std::ostream& stream) = 0;
    };
}

#endif