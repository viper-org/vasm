#ifndef VASM_CODEGEN_ELF_H
#define VASM_CODEGEN_ELF_H 1

#include "vasm/codegen/IOutputFormat.h"

#include <concepts>
#include <vector>
#include <set>
#include <vector>

namespace lexing
{
    class Token;
}

namespace codegen
{   
    class ELFFormat : public IOutputFormat
    {
    public:
        explicit ELFFormat(std::string_view fileName);

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
        class ELFSection
        {
        public:
            ELFSection(std::string_view name, int type, std::int64_t flags, int link, int info, std::int64_t align, std::int64_t entrySize, Section section);
            explicit ELFSection(Section section);

            void write(std::unsigned_integral auto data);
            void write(const char* data, size_t size);
            void write(std::string_view data);

            std::vector<char> mBuffer;

            std::string mName;
            int mNameIdx;
            int mType;
            std::int64_t mFlags;
            int mLink;
            int mInfo;
            std::int64_t mAlign;
            std::int64_t mEntrySize;
            Section mSection;

            [[nodiscard]] size_t size() const;

            int& length(); // should only be used for symtab
        };

        struct ELFSymbol
        {
            std::string name;
            
            std::uint32_t strtabIndex;
            std::uint8_t link;
            std::uint8_t type;
            std::uint16_t sectionIndex;
             std::uint64_t value;
             std::uint64_t size;

            bool external;
            int index;

            bool operator==(const ELFSymbol& other)
            {
                return name == other.name;
            }
        };

        struct ELFSymbolComparator
        {
            bool operator()(const ELFSymbol& a, const ELFSymbol& b) const
            {
                return a.index <= b.index;
            }
        };
        using Symbols = std::set<ELFSymbol, ELFSymbolComparator>;

        ELFSection* getSection(std::string_view name);
        ELFSection* getSection(Section section);

        ELFSection* createSection(Section section);

        ELFSection* getOrCreateSection(Section section);

        void incrementGlobalSymbolIndex();

        std::vector<ELFSection> mSections;

        std::set<ELFSymbol, ELFSymbolComparator> mLocalSymbols;
        std::set<ELFSymbol, ELFSymbolComparator> mGlobalSymbols;

        std::string_view mFileName;

    };
}

#endif