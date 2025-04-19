#ifndef VASM_CODEGEN_OUTPUT_FORMAT_H
#define VASM_CODEGEN_OUTPUT_FORMAT_H 1

#include "vasm/codegen/Opcodes.h"

#include <cstdint>
#include <string>
#include <ostream>

namespace codegen
{
    using Global = bool;

    using Section = std::string;

    struct SectionInfo
    {
        virtual ~SectionInfo() = default;
    };

    class IOutputFormat
    {
    public:
        virtual ~IOutputFormat() = default;

        virtual void write(std::uint8_t  data, std::string section, std::uint64_t offset = -1, bool overwrite = false) = 0;
        virtual void write(std::uint16_t data, std::string section, std::uint64_t offset = -1, bool overwrite = false) = 0;
        virtual void write(std::uint32_t data, std::string section, std::uint64_t offset = -1, bool overwrite = false) = 0;
        virtual void write(std::uint64_t data, std::string section, std::uint64_t offset = -1, bool overwrite = false) = 0;

        virtual size_t getPosition(std::string section) = 0;
        virtual size_t getSectionStart(std::string section) = 0;

        virtual void addSymbol(const std::string& name, std::uint64_t value, std::string section, bool isGlobal) = 0;
        virtual void addExternSymbol(const std::string& name) = 0;
        [[nodiscard]] virtual bool hasSymbol(const std::string& name) const = 0;
        [[nodiscard]] virtual std::pair<std::uint64_t, bool> getSymbol(const std::string& name) const = 0;
        virtual std::string getSymbolAfter(const std::string& name) const = 0;

        virtual void createSection(SectionInfo* info) = 0;
        virtual std::string getSymbolSection(std::string_view name) const = 0;
        virtual std::string getSection(std::string_view name) = 0;
        virtual std::string getCodeSectionName() = 0;
        
        virtual void relocSymbol(const std::string& name, const std::string& location, std::string section, int offset = 0, int addend = 0) = 0;
        virtual void patchForwardSymbol(const std::string& name, std::string section, OperandSize size, int location, int origin) = 0;

        virtual void print(std::ostream& stream) = 0;
    };
}

#endif