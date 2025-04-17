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

        void write(std::uint8_t  data, std::string section, std::uint64_t offset) override;
        void write(std::uint16_t data, std::string section, std::uint64_t offset) override;
        void write(std::uint32_t data, std::string section, std::uint64_t offset) override;
        void write(std::uint64_t data, std::string section, std::uint64_t offset) override;

        size_t getPosition(std::string section) override;
        size_t getSectionStart(std::string section) override;

        void addSymbol(const std::string& name, std::uint64_t value, std::string section, bool isGlobal) override;
        void addExternSymbol(const std::string& name) override;
        [[nodiscard]] std::pair<std::uint64_t, bool> getSymbol(const std::string& name) const override;
        std::string getSymbolAfter(const std::string& name) const override;

        virtual void createSection(SectionInfo* info) override;
        virtual std::string getSymbolSection(std::string_view name) const override;
        virtual std::string getSection(std::string_view name) override;
        virtual std::string getCodeSectionName() override;

        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, const std::string& location, std::string section, int offset, int addend) override;
        void patchForwardSymbol(const std::string& name, std::string section, OperandSize size, int location, int origin) override;

        void print(std::ostream& stream) override;
    private:
        class ELFSection
        {
        public:
            ELFSection(std::string_view name, int type, long flags, int link, int info, long align, long entrySize);
            explicit ELFSection(std::string_view section);

            void write(std::unsigned_integral auto data);
            void write(const char* data, size_t size, std::uint64_t offset = -1);
            void write(std::string_view data);

            std::vector<char> mBuffer;

            std::string mName;
            int mNameIdx;
            int mType;
            long mFlags;
            int mLink;
            int mInfo;
            long mAlign;
            long mEntrySize;

            [[nodiscard]] size_t size() const;

            int& length(); // should only be used for symtab
        };

        struct ELFSymbol
        {
            std::string name;
            
            unsigned int strtabIndex;
            unsigned char link;
            unsigned char type;
            unsigned short sectionIndex;
            unsigned long value;
            unsigned long size;

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

        struct ELFRelocation
        {
            std::string name;
            std::string location;
            std::string section;
            int offset;
            int addend;

            std::size_t position;
        };

        ELFSection* getElfSection(std::string_view name);

        ELFSection* createSection(std::string_view section);

        ELFSection* getOrCreateSection(std::string section);

        void incrementGlobalSymbolIndex();

        void doRelocations();

        std::vector<ELFSection> mSections;

        std::set<ELFSymbol, ELFSymbolComparator> mLocalSymbols;
        std::set<ELFSymbol, ELFSymbolComparator> mGlobalSymbols;

        std::vector<ELFRelocation> mRelocations;

        std::string_view mFileName;
    };

    struct ELFSectionInfo : public SectionInfo
    {
        ELFSectionInfo(std::string name, int type, long flags, int link, int info, long align, long entrySize);

        std::string name;
        int type;
        long flags;
        int link;
        int info;
        long align;
        long entrySize;
    };
}

#endif