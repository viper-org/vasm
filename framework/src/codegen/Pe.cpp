#include "vasm/codegen/Pe.h"

#include <cstring>
#include <cassert>
#include <bit>

namespace codegen {
    constexpr uint16_t IMAGE_FILE_MACHINE_AMD64 = 0x8664;
    
    constexpr uint16_t IMAGE_FILE_LARGE_ADDRESS_AWARE = 0x20;
    
    constexpr uint32_t IMAGE_SCN_CNT_CODE = 0x20;
    constexpr uint32_t IMAGE_SCN_CNT_INITIALIZED_DATA = 0x40;
    constexpr uint32_t IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x80;
    constexpr uint32_t IMAGE_SCN_LNK_NRELOC_OVFL = 0x1000000;
    constexpr uint32_t IMAGE_SCN_MEM_EXECUTE = 0x20000000;
    constexpr uint32_t IMAGE_SCN_MEM_READ = 0x40000000;
    constexpr uint32_t IMAGE_SCN_MEM_WRITE = 0x80000000;
    
    constexpr uint8_t IMAGE_SYM_CLASS_EXTERNAL = 2;
    constexpr uint8_t IMAGE_SYM_CLASS_STATIC = 3;
    constexpr uint8_t IMAGE_SYM_CLASS_LABEL = 6;
    
    constexpr uint16_t IMAGE_REL_AMD64_ADDR64 = 1;
    
    constexpr size_t COFF_HEADER_SIZE = 20;
    constexpr size_t SECTION_HEADER_SIZE = 40;
    constexpr size_t SYMBOL_SIZE = 18;
    constexpr size_t COFF_RELOC_SIZE = 10;
    
    PEFormat::PEFormat(std::string_view fileName)
        : mFileName{fileName}
    {
    }
    
    void PEFormat::write(unsigned char data, Section section)
    {
        PESection* peSection = getSection(section);
        if (!peSection)
        {
            peSection = createSection(section);
        }
        peSection->write(reinterpret_cast<const char*>(&data), sizeof(data));
    }
    
    void PEFormat::write(unsigned short data, Section section)
    {
        PESection* peSection = getSection(section);
        if (!peSection)
        {
            peSection = createSection(section);
        }
        peSection->write(reinterpret_cast<const char*>(&data), sizeof(data));
    }
    
    void PEFormat::write(unsigned int data, Section section)
    {
        PESection* peSection = getSection(section);
        if (!peSection)
        {
            peSection = createSection(section);
        }
        peSection->write(reinterpret_cast<const char*>(&data), sizeof(data));
    }
    
    void PEFormat::write(unsigned long data, Section section)
    {
        PESection* peSection = getSection(section);
        if (!peSection)
        {
            peSection = createSection(section);
        }
        peSection->write(reinterpret_cast<const char*>(&data), sizeof(data));
    }
    
    size_t PEFormat::getPosition(Section section)
    {
        PESection* peSection = getSection(section);
        if (!peSection)
        {
            peSection = createSection(section);
        }
        return peSection->mBuffer.size();
    }
    
    size_t PEFormat::getSectionStart(Section)
    {
        return 0;
    }
    
    static constexpr uint32_t alignToCharacteristic(uint32_t align) {
        if (align == 0) {
            return 0;
        }

        // Make sure align is power of two and at max 8192 bytes
        assert((align & (align - 1)) == 0);
        assert(align <= 8192);
        return std::bit_width(align) << 20;
    }
    
    PEFormat::PESection::PESection(Section section)
        : mNameOffset {0}, mSection {section} {
        switch (section)
        {
            case Section::Text:
                mName = ".text";
                mAlign = 16;
                mCharacteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE | alignToCharacteristic(mAlign);
                break;
            case Section::Data:
                mName = ".data";
                mAlign = 16;
                mCharacteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | alignToCharacteristic(mAlign);
                break;
            case Section::Other:
                break;
        }
    }
    
    void PEFormat::PESection::write(std::unsigned_integral auto data)
    {
        for (size_t i = 0; i < sizeof(data) * 8; i += 8) {
            mBuffer.push_back(data >> i);
        }
    }

    void PEFormat::PESection::write(const char* data, size_t size)
    {
        for (; size; --size) {
            mBuffer.push_back(*data++);
        }
    }

    void PEFormat::PESection::write(std::string_view data)
    {
        for (size_t i = 0; i < data.size() + 1; ++i) {
            mBuffer.push_back(data[i]);
        }
    }
    
    PEFormat::PESection* PEFormat::getSection(Section section) {
        for (auto& sect : mSections)
        {
            if (sect.mSection == section)
            {
                return &sect;
            }
        }
        return nullptr;
    }
    
    PEFormat::PESection* PEFormat::createSection(Section section) {
        mSections.emplace_back(section);
        
        PESection* newSection = &mSections.back();
        
        if (newSection->mName.size() > 8)
        {
            newSection->mNameOffset = mStringTable.size();
            mStringTable += newSection->mName + '\0';
        }
        
        return newSection;
    }
    
    PEFormat::PESection* PEFormat::getOrCreateSection(Section section)
    {
        PESection* sect = getSection(section);
        if (!sect)
        {
            return createSection(section);
        }
        else
        {
            return sect;
        }
    }
    
    void PEFormat::addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal)
    {
        getOrCreateSection(section);
        
        size_t sectionIndex = 0;
        for (; sectionIndex < mSections.size(); ++sectionIndex)
        {
            if (mSections[sectionIndex].mSection == section)
            {
                break;
            }
        }
        
        assert(sectionIndex + 1 <= INT16_MAX);
        
        // mType 0 == not function, 0x20 == function
        PESymbol symbol {
             .mValue = static_cast<uint32_t>(value),
             .mSectionNumber = static_cast<int16_t>(sectionIndex + 1),
             .mType = 0,
             .mStorageClass = IMAGE_SYM_CLASS_EXTERNAL,
             .mNumberOfAuxSymbols = 0
        };
        if (name.size() < 8)
        {
            memcpy(symbol.mShortName.name, name.c_str(), name.size() + 1);
        }
        else
        {
            symbol.mShortName.offset.zeros = 0;
            symbol.mShortName.offset.offset = mStringTable.size();
            mStringTable += name + '\0';
        }
        
        mSymbolTable.push_back(symbol);
        mSymbolIndices[name] = mSymbolTable.size() - 1;
    }

    void PEFormat::addExternSymbol(const std::string& name)
    {
        // TODO: Implement
    }
    
    [[nodiscard]] std::pair<unsigned long, bool> PEFormat::getSymbol(const std::string& name) const
    {
        return std::make_pair(mSymbolTable[mSymbolIndices.at(name)].mValue, false);
    }
    
    [[nodiscard]] bool PEFormat::hasSymbol(const std::string& name) const
    {
        return mSymbolIndices.contains(name);
    }
    
    void PEFormat::relocSymbol(const std::string& name, const std::string& location, Section section, int offset)
    {
        // TODO: Add location specification
        PESection* sect = getOrCreateSection(section);
        
        uint16_t symbolIndex = mSymbolIndices[name];
        
        PESection::Relocation reloc {
            .mVirtualAddress = static_cast<uint32_t>(getPosition(section) + offset),
            .mSymbolTableIndex = symbolIndex,
            .mType = IMAGE_REL_AMD64_ADDR64
        };
        sect->mRelocations.push_back(reloc);
    }
    
    template<typename T>
    static inline void PEWrite(std::ostream& stream, T data)
    {
        stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
    }
    
    static inline void PEWrite(std::ostream& stream, const char* data, size_t size)
    {
        stream.write(data, static_cast<std::streamsize>(size));
    }

    void PEFormat::print(std::ostream& stream)
    {
        size_t symbolTableOffset = COFF_HEADER_SIZE + mSections.size() * SECTION_HEADER_SIZE;
        size_t symbolTableSize = mSymbolTable.size() * SYMBOL_SIZE;
        
        // COFF header
        // Machine
        PEWrite(stream, IMAGE_FILE_MACHINE_AMD64);
        // Number of sections
        PEWrite(stream, static_cast<uint16_t>(mSections.size()));
        // Low 32 bits of file creation time in seconds since 01.01.1970
        PEWrite(stream, static_cast<uint32_t>(0));
        // Symbol table file offset
        PEWrite(stream, static_cast<uint32_t>(symbolTableOffset));
        // Number of symbols
        PEWrite(stream, static_cast<uint32_t>(mSymbolTable.size()));
        // Size of optional header (only needed for images)
        PEWrite(stream, static_cast<uint16_t>(0));
        // Characteristics (large address aware -> can handle addresses higher than 2gb)
        PEWrite(stream, IMAGE_FILE_LARGE_ADDRESS_AWARE);
        
        size_t usableStart = symbolTableOffset + symbolTableSize + mStringTable.size();
        
        char null_char = 0;
        for (auto& sect : mSections)
        {
            if (sect.mName.size() <= 8)
            {
                PEWrite(stream, sect.mName.c_str(), sect.mName.size() + 1);
                for (size_t i = sect.mName.size() + 1; i < 8; ++i)
                {
                    PEWrite(stream, null_char);
                }
            }
            else
            {
                std::string name = "/" + std::to_string(sect.mNameOffset);
                assert(name.size() + 1 <= 8);
                PEWrite(stream, name.c_str(), name.size() + 1);
                for (size_t i = name.size() + 1; i < 8; ++i)
                {
                    PEWrite(stream, null_char);
                }
            }
            
            if (usableStart % sect.mAlign) {
                sect.mDataAlignBytes = sect.mAlign - (usableStart % sect.mAlign);
                usableStart += sect.mDataAlignBytes;
            }
            
            // VirtualSize
            PEWrite(stream, static_cast<uint32_t>(0));
            // VirtualAddress
            PEWrite(stream, static_cast<uint32_t>(0));
            // SizeOfRawData
            PEWrite(stream, static_cast<uint32_t>(sect.mBuffer.size()));
            // PointerToRawData
            PEWrite(stream, static_cast<uint32_t>(usableStart));
            
            // PointerToRelocations
            if (!sect.mRelocations.empty())
            {
                PEWrite(stream, static_cast<uint32_t>(usableStart + sect.mBuffer.size()));
            }
            else
            {
                PEWrite(stream, static_cast<uint32_t>(0));
            }
            // PointerToLinenumbers
            PEWrite(stream, static_cast<uint32_t>(0));
            // NumberOfRelocations
            PEWrite(stream, static_cast<uint16_t>(sect.mRelocations.size()));
            // NumberOfLinenumbers
            PEWrite(stream, static_cast<uint16_t>(0));
            // Characteristics
            PEWrite(stream, sect.mCharacteristics);
            
            usableStart += sect.mBuffer.size() + sect.mRelocations.size() * COFF_RELOC_SIZE;
        }
        
        for (const auto& sym : mSymbolTable) {
            PEWrite(stream, sym.mShortName.name, 8);
            PEWrite(stream, sym.mValue);
            PEWrite(stream, sym.mSectionNumber);
            PEWrite(stream, sym.mType);
            PEWrite(stream, sym.mStorageClass);
            PEWrite(stream, sym.mNumberOfAuxSymbols);
        }
        
        PEWrite(stream, mStringTable.c_str(), mStringTable.size());
        
        for (const auto& sect : mSections) {
            for (size_t i = 0; i < sect.mDataAlignBytes; ++i) {
                PEWrite(stream, null_char);
            }
            
            PEWrite(stream, sect.mBuffer.data(), sect.mBuffer.size());
            for (const auto& reloc : sect.mRelocations) {
                PEWrite(stream, reloc.mVirtualAddress);
                PEWrite(stream, reloc.mSymbolTableIndex);
                PEWrite(stream, reloc.mType);
            }
        }
    }
}
