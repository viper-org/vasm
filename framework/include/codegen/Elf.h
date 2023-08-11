#ifndef VASM_CODEGEN_ELF_H
#define VASM_CODEGEN_ELF_H 1

#include "codegen/IOutputFormat.h"

#include <concepts>
#include <vector>
#include <unordered_map>
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

        void write(unsigned char  data, Section section) override;
        void write(unsigned short data, Section section) override;
        void write(unsigned int   data, Section section) override;
        void write(unsigned long  data, Section section) override;

        size_t getPosition(Section section) override;
        size_t getSectionStart(Section section) override;

        void addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal) override;
        [[nodiscard]] unsigned long getSymbol(const std::string& name) const override;
        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, Section section, int offset) override;

        void print(std::ostream& stream) override;
    private:
        class ELFSection
        {
        public:
            ELFSection(std::string_view name, int type, long flags, int link, int info, long align, long entrySize, Section section);
            explicit ELFSection(Section section);

            void write(std::unsigned_integral auto data);
            void write(const char* data, size_t size);
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
            Section mSection;

            [[nodiscard]] size_t size() const;

            int& length(); // should only be used for symtab
        };

        ELFSection* getSection(std::string_view name);
        ELFSection* getSection(Section section);

        ELFSection* createSection(Section section);

        ELFSection* getOrCreateSection(Section section);

        std::vector<ELFSection> mSections;

        std::unordered_map<std::string, unsigned long> mSymbols;

        std::string_view mFileName;
    };
}

#endif