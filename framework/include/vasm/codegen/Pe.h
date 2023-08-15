#ifndef VASM_CODEGEN_PE_H
#define VASM_CODEGEN_PE_H 1

#include "vasm/codegen/IOutputFormat.h"

#include <cstdint>
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
    class PEFormat : public IOutputFormat
    {
    public:
        explicit PEFormat(std::string_view fileName);

        void write(unsigned char  data, Section section) override;
        void write(unsigned short data, Section section) override;
        void write(unsigned int   data, Section section) override;
        void write(unsigned long  data, Section section) override;

        size_t getPosition(Section section) override;
        size_t getSectionStart(Section section) override;

        void addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal) override;
        void addExternSymbol(const std::string& name) override;
        [[nodiscard]] std::pair<unsigned long, bool> getSymbol(const std::string& name) const override;
        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, const std::string& location, Section section, int offset) override;

        void print(std::ostream& stream) override;
    private:
        class PESection
        {
        public:
            struct Relocation
            {
                uint32_t mVirtualAddress;
                uint32_t mSymbolTableIndex;
                uint16_t mType;
            };
        
            explicit PESection(Section section);
        
            void write(std::unsigned_integral auto data);
            void write(const char* data, size_t size);
            void write(std::string_view data);

            std::vector<char> mBuffer;
            std::vector<Relocation> mRelocations;
        
            std::string mName;
            size_t mNameOffset;
            size_t mAlign {};
            size_t mDataAlignBytes {};
            uint32_t mCharacteristics;
            Section mSection;
        };
        
        struct PESymbol
        {
            union {
                char name[8];
                struct {
                    uint32_t zeros;
                    uint32_t offset;
                } offset;
            } mShortName;
            uint32_t mValue;
            int16_t mSectionNumber;
            uint16_t mType;
            uint8_t mStorageClass;
            uint8_t mNumberOfAuxSymbols;
        };
        
        PESection* getSection(Section section);
        PESection* createSection(Section section);
        PESection* getOrCreateSection(Section section);
    
        std::vector<PESection> mSections;
    
        std::string mStringTable;
        std::unordered_map<std::string, size_t> mSymbolIndices;
        std::vector<PESymbol> mSymbolTable;

        std::string_view mFileName;
    };
}

#endif