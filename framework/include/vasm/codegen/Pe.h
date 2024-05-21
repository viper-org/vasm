#ifndef VASM_CODEGEN_PE_H
#define VASM_CODEGEN_PE_H 1

#include "vasm/codegen/IOutputFormat.h"

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

        void write(std::uint8_t  data, std::string section) override;
        void write(std::uint16_t data, std::string section) override;
        void write(std::uint32_t data, std::string section) override;
        void write(std::uint64_t data, std::string section) override;

        size_t getPosition(std::string section) override;
        size_t getSectionStart(std::string section) override;

        void addSymbol(const std::string& name, std::uint64_t value, std::string section, bool isGlobal) override;
        void addExternSymbol(const std::string& name) override;
        [[nodiscard]] std::pair<std::uint64_t, bool> getSymbol(const std::string& name) const override;

        virtual std::string getSymbolSection(std::string_view name) const override;
        virtual std::string getSection(std::string_view name) override;
        virtual std::string getCodeSectionName() override;

        [[nodiscard]] bool hasSymbol(const std::string& name) const override;
        void relocSymbol(const std::string& name, const std::string& location, std::string section, int offset, int addend) override;
        void patchForwardSymbol(const std::string& name, std::string section, OperandSize size, int location, int origin) override;

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
        
            explicit PESection(std::string section);
        
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
        
        PESection* getSection(std::string section);
        PESection* createSection(std::string section);
        PESection* getOrCreateSection(std::string section);
    
        std::vector<PESection> mSections;
    
        std::string mStringTable;
        std::unordered_map<std::string, size_t> mSymbolIndices;
        std::vector<PESymbol> mSymbolTable;

        std::string_view mFileName;
    };
}

#endif