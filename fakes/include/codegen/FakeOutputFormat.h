#ifndef VASM_FAKES_CODEGEN_FAKE_OUTPUT_FORMAT_H
#define VASM_FAKES_CODEGEN_FAKE_OUTPUT_FORMAT_H 1

#include "vasm/codegen/IOutputFormat.h"

#include <format>
#include <vector>

namespace codegen
{
    namespace fakes
    {
        struct FakeSymbol
        {
            std::string name;
            unsigned long value{ 0 };
            Section section;
            bool isGlobal{ true };

            bool operator==(const FakeSymbol& other) const = default;
        };
        struct FakeRelocation
        {
            std::string name;
            Section section;

            bool operator==(const FakeRelocation& other) const = default;
        };

        class FakeOutputFormat : public IOutputFormat
        {
        public:
            FakeOutputFormat();

            void write(std::uint8_t  data, Section section) override;
            void write(std::uint16_t data, Section section) override;
            void write(std::uint32_t data, Section section) override;
            void write(std::uint64_t data, Section section) override;

            size_t getPosition(Section section) override;
            size_t getSectionStart(Section section) override;

            void addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal) override;
            void addExternSymbol(const std::string& name) override;
            [[nodiscard]] std::pair<unsigned long, bool> getSymbol(const std::string& name) const override;
            virtual Section getSymbolSection(std::string_view name) const override;
            virtual Section getSection(std::string_view name) override;
            [[nodiscard]] bool hasSymbol(const std::string& name) const override;
            void relocSymbol(const std::string& name, const std::string& location, Section section, int offset, int addend) override;
            void patchForwardSymbol(const std::string& name, Section section, OperandSize size, int location, int origin) override;

            void print(std::ostream& stream) override;

            const std::vector<unsigned char>&  getCode()        const;
            const std::vector<unsigned char>&  getData()        const;
            const std::vector<FakeSymbol>&     getSymbols()     const;
            const std::vector<FakeRelocation>& getRelocations() const;
        
        private:
            std::vector<unsigned char> mCode;
            std::vector<unsigned char> mData;
            
            std::vector<FakeSymbol> mSymbols;
            std::vector<FakeRelocation> mRelocations;
        };
    }
}

template<>
struct std::formatter<codegen::fakes::FakeSymbol> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(codegen::fakes::FakeSymbol fakeSymbol, FormatContext& context) const
    {
        return formatter<string>::format(std::format("{{{}, {}, {}, {}}}", fakeSymbol.name, fakeSymbol.value, (int)fakeSymbol.section, fakeSymbol.isGlobal), context);
    }
};

template<>
struct std::formatter<codegen::fakes::FakeRelocation> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(codegen::fakes::FakeRelocation fakeRelocation, FormatContext& context) const
    {
        return formatter<string>::format(std::format("{{{}, {}}}", fakeRelocation.name, (int)fakeRelocation.section), context);
    }
};

#endif // VASM_FAKES_CODEGEN_FAKE_OUTPUT_FORMAT_H