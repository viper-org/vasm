#ifndef VASM_CODEGEN_ELF_H
#define VASM_CODEGEN_ELF_H 1

#include <codegen/OutputFormat.h>

#include <concepts>
#include <sstream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace Lexing
{
    class Token;
}

namespace Codegen
{   
    class ELFFormat : public OutputFormat
    {
    public:
        ELFFormat(const std::string& fileName);

        void write(unsigned char  const data, Section const section) override;
        void write(unsigned short const data, Section const section) override;
        void write(unsigned int   const data, Section const section) override;
        void write(unsigned long  const data, Section const section) override;

        int getPosition(Section const section) override;
        int getSectionStart(Section const section) override;

        void addSymbol(const std::string& name, unsigned long value, Section const section, bool isGlobal) override;
        unsigned long getSymbol(const std::string& name) override;
        void relocSymbol(const std::string& name, Section const section) override;

        void print(std::ostream& stream) override;
    private:
        class ELFSection
        {
        public:
            ELFSection(const std::string& name, int type, long flags, int link, int info, long align, long entrySize, Section section);
            ELFSection(Section section);

            void write(std::unsigned_integral auto const data);
            void write(char const* data, int size);
            void write(std::string_view data);

            std::stringstream mBuffer;

            std::string mName;
            int mNameIdx;
            int mType;
            long mFlags;
            int mLink;
            int mInfo;
            long mAlign;
            long mEntrySize;
            Section mSection;

            unsigned int size() const;

            int& length(); // should only be used for symtab
        };

        ELFSection* getSection(const std::string& name);
        ELFSection* getSection(Section const section);

        ELFSection* createSection(Section const section);

        ELFSection* getOrCreateSection(Section const section);

        std::vector<ELFSection> mSections;

        std::unordered_map<std::string, unsigned long> mSymbols;

        const std::string& mFileName;
    };
}

#endif