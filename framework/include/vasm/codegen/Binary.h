#ifndef VASM_CODEGEN_BINARY_H
#define VASM_CODEGEN_BINARY_H 1

#include "vasm/codegen/IOutputFormat.h"

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

        void write(std::uint8_t      data, Section section) override;
        void write(std::uint16_t     data, Section section) override;
        void write(std::uint32_t       data, Section section) override;
        void write(std::uint64_t data, Section section) override;

        size_t getPosition(Section section) override;
        size_t getSectionStart(Section section) override;

        void addSymbol(const std::string& name, std::uint64_t value, Section section, bool isGlobal) override;
        void addExternSymbol(const std::string& name) override;
        [[nodiscard]] std::pair<std::uint64_t, bool> getSymbol(const std::string& name) const override;
        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, const std::string& location, Section section, int offset) override;
        void patchForwardSymbol(const std::string& name, Section section, OperandSize size, int location, int origin) override;

        void print(std::ostream& stream) override;
    private:
        std::vector<std::uint8_t> mBuffer;
        std::unordered_map<std::string,  std::uint64_t> mSymbols;
    };
}

#endif