#ifndef VASM_CODEGEN_BINARY_H
#define VASM_CODEGEN_BINARY_H 1

#include "vasm/codegen/IOutputFormat.h"

#include <memory>
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

        void write(std::uint8_t  data, std::string section, std::uint64_t offset, bool overwrite) override;
        void write(std::uint16_t data, std::string section, std::uint64_t offset, bool overwrite) override;
        void write(std::uint32_t data, std::string section, std::uint64_t offset, bool overwrite) override;
        void write(std::uint64_t data, std::string section, std::uint64_t offset, bool overwrite) override;

        size_t getPosition(std::string section) override;
        size_t getSectionStart(std::string section) override;

        void addSymbol(const std::string& name, std::uint64_t value, std::string section, bool isGlobal) override;
        void addExternSymbol(const std::string& name) override;
        [[nodiscard]] std::pair<std::uint64_t, bool> getSymbol(const std::string& name) const override;
        std::string getSymbolAfter(const std::string& name) const override;

        virtual void createSection(SectionInfo* info) override;
        virtual std::string getSymbolSection(std::string_view name) override;
        virtual std::string getSection(std::string_view name) override;
        virtual std::string getCodeSectionName() override;

        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, const std::string& location, std::string section, int offset, int addend) override;
        void patchForwardSymbol(const std::string& name, std::string section, OperandSize size, int location, int origin) override;

        void print(std::ostream& stream) override;
        std::unique_ptr<unsigned char[]> loadjit();
    private:
        std::vector<unsigned char> mBuffer;
        std::unordered_map<std::string, std::uint64_t> mSymbols;
    };
}

#endif