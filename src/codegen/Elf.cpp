#include <codegen/Elf.h>

namespace Codegen
{
    // TODO: Make this into an enum or similar
    constexpr char const* ELF_MAGIC = "\x{7f}ELF";
    constexpr char ELF_64 = 2;
    constexpr char ELF_LITTLE_ENDIAN = 1;
    constexpr char ELF_EI_VERSION = 1;
    constexpr char ELF_SYSV_ABI = 0;
    constexpr char ELF_ABI_VERSION = 1;
    constexpr char const* ELF_PAD = "\0\0\0\0\0\0\0";

    constexpr short ELF_OBJECT = 1;
    constexpr short ELF_AMD64 = 0x3E;
    constexpr int ELF_VERSION = 1;
    constexpr long ELF_ENTRY = 0;
    constexpr long ELF_PHOFF = 0;
    constexpr long ELF_SHOFF = 0x40;
    constexpr int ELF_FLAGS = 0;
    constexpr short ELF_EHSIZE = 64;
    constexpr short ELF_PHSIZE = 0;
    constexpr short ELF_PHNUM = 0;
    constexpr short ELF_SHSIZE = 0x40;
    constexpr short ELF_SHNUM = 5;
    constexpr short ELF_SHSTRNDX = 2;
    
    constexpr int TYPE_NULL = 0;
    constexpr int TYPE_PROGBITS = 1;
    constexpr int TYPE_SYMTAB = 2;
    constexpr int TYPE_STRTAB = 3;
    constexpr int TYPE_RELA = 4;

    constexpr long SECTION_ADDR = 0;

    constexpr unsigned char SYM_LOCAL = 0;
    constexpr unsigned char SYM_GLOBAL = 1 << 4;
    constexpr unsigned char SYM_DEFAULT = 0;
    constexpr unsigned char SYM_FUNC = 2;
    constexpr unsigned char SYM_SECTION = 3;
    constexpr unsigned char SYM_FILE = 4;

    constexpr unsigned long SYM_VALUE_0 = 0;
    constexpr unsigned long SYM_SIZE_0 = 0;

    constexpr unsigned short SYM_ABS = 0xFFF1;

    ELFFormat::ELFFormat(const std::string& fileName)
        :mFileName(fileName)
    {
        mSections.push_back(ELFSection("", TYPE_NULL, 0, 0, 0, 0, 0, Section::Other));
        mSections.push_back(ELFSection(".shstrtab", TYPE_STRTAB, 0, 0, 0, 1, 0, Section::Other));
        mSections.push_back(ELFSection(".symtab", TYPE_SYMTAB, 0, 3, 0, 8, 24, Section::Other));
        mSections.push_back(ELFSection(".strtab", TYPE_STRTAB, 0, 0, 0, 1, 0, Section::Other));

        mSections[3].write("", 1); // NULL string
        mSections[3].write(fileName.data(), fileName.length() + 1); // File name string

        // NULL symbol
        mSections[2].write((unsigned int)0);
        mSections[2].write((unsigned char)0);
        mSections[2].write(SYM_DEFAULT);
        mSections[2].write((unsigned short)0);
        mSections[2].write(SYM_VALUE_0);
        mSections[2].write(SYM_SIZE_0);
        mSections[2].mInfo++;

        // File symbol
        mSections[2].write((unsigned int)1);
        mSections[2].write(SYM_FILE);
        mSections[2].write(SYM_DEFAULT);
        mSections[2].write(SYM_ABS);
        mSections[2].write(SYM_VALUE_0);
        mSections[2].write(SYM_SIZE_0);
        mSections[2].mInfo++;
    }


    void ELFFormat::write(unsigned char const data, Section const section)
    {
        ELFSection* elfSection = getSection(section);
        if (!elfSection)
        {
            elfSection = createSection(section);
        }
        elfSection->write((char const*)&data, sizeof(data));
    }

    void ELFFormat::write(unsigned short const data, Section const section)
    {
        ELFSection* elfSection = getSection(section);
        if (!elfSection)
        {
            elfSection = createSection(section);
        }
        elfSection->write((char const*)&data, sizeof(data));
    }

    void ELFFormat::write(unsigned int const data, Section const section)
    {
        ELFSection* elfSection = getSection(section);
        if (!elfSection)
        {
            elfSection = createSection(section);
        }
        elfSection->write((char const*)&data, sizeof(data));
    }

    void ELFFormat::write(unsigned long const data, Section const section)
    {
        ELFSection* elfSection = getSection(section);
        if (!elfSection)
        {
            elfSection = createSection(section);
        }
        elfSection->write((char const*)&data, sizeof(data));
    }


    int ELFFormat::getPosition(Section const section)
    {
        ELFSection* elfSection = getSection(section);
        if (!elfSection)
        {
            elfSection = createSection(section);
        }
        return elfSection->mBuffer.view().length();
    }

    int ELFFormat::getSectionStart(Section const)
    {
        return 0;
    }

    void ELFFormat::addSymbol(const std::string& name, unsigned long value, Section const section, bool isGlobal)
    {
        ELFSection* strtab = getSection(".strtab");
        unsigned int strtabIndex = strtab->size();
        strtab->write(name);

        getOrCreateSection(section);

        ELFSection* symtab = getSection(".symtab");

        symtab->write(strtabIndex);
        symtab->write(isGlobal ? SYM_GLOBAL : SYM_LOCAL);
        symtab->write(SYM_DEFAULT);
        for (unsigned short sectionIndex = 0; sectionIndex < mSections.size(); sectionIndex++)
        {
            if (mSections[sectionIndex].mSection == section)
            {
                symtab->write(sectionIndex);
                break;
            }
        }
        symtab->write(value);
        symtab->write(SYM_SIZE_0);
        if (!isGlobal)
        {
            symtab->length()++;
        }

        mSymbols[name] = value;
    }

    unsigned long ELFFormat::getSymbol(const std::string& name)
    {
        return mSymbols.at(name);
    }

    void ELFFormat::relocSymbol(const std::string& name, Section const section)
    {
        ELFSection* sect = getOrCreateSection(section);
        ELFSection* rela = getSection(".rela" + sect->mName);
        if(!rela)
        {
            int symtabIndex;
            int sectionIndex = 0;
            for (symtabIndex = 0; symtabIndex < mSections.size(); symtabIndex++)
            {
                if (mSections[symtabIndex].mName == ".symtab")
                {
                    break;
                }
            }
            for (sectionIndex = 0; sectionIndex < mSections.size(); sectionIndex++)
            {
                if (mSections[sectionIndex].mName == sect->mName)
                {
                    break;
                }
            }

            mSections.push_back(ELFSection(".rela" + sect->mName, TYPE_RELA, 0, symtabIndex, sectionIndex, 8, 24, Section::Other));

            rela = &mSections.back();
        }

        unsigned long symbol = mSymbols.at(name);
        
        rela->write((unsigned long)getPosition(section));
        rela->write((unsigned long)0x200000001);
        rela->write(symbol);
    }


    ELFFormat::ELFSection::ELFSection(const std::string& name, int type, long flags, int link, int info, long align, long entrySize, Section section)
        :mBuffer(std::stringstream(std::ios::out | std::ios::binary)), mName(name), mType(type), mFlags(flags), mLink(link), mInfo(info), mAlign(align), mEntrySize(entrySize), mSection(section)
    {
    }

    ELFFormat::ELFSection::ELFSection(Section section)
        :mBuffer(std::stringstream(std::ios::out | std::ios::binary)), mSection(section)
    {
        switch(section)
        {
            case Section::Text:
                mName = ".text";
                mType = TYPE_PROGBITS;
                mFlags = 6;
                mLink = 0;
                mInfo = 0;
                mAlign = 16;
                mEntrySize = 0;
                break;
            case Section::Data:
                mName = ".text";
                mType = TYPE_PROGBITS;
                mFlags = 2;
                mLink = 0;
                mInfo = 0;
                mAlign = 16;
                mEntrySize = 0;
                break;
            default:
                break;
        }
    }

    void ELFFormat::ELFSection::write(std::unsigned_integral auto const data)
    {
        mBuffer.write((char const*)&data, sizeof(data));
    }

    void ELFFormat::ELFSection::write(char const* data, int size)
    {
        mBuffer.write(data, size);
    }

    void ELFFormat::ELFSection::write(std::string_view data)
    {
        mBuffer.write(data.data(), data.length() + 1);
    }

    unsigned int ELFFormat::ELFSection::size() const
    {
        return mBuffer.view().length();
    }

    int& ELFFormat::ELFSection::length()
    {
        return mInfo;
    }


    template <typename T>
    static inline constexpr void WriteELF(std::ostream& stream, T data)
    {
        stream.write((char*)&data, sizeof(T));
    }

    static inline void WriteELF(std::ostream& stream, char const* data, int size)
    {
        stream.write(data, size);
    }

    void ELFFormat::print(std::ostream& stream)
    {
        WriteELF(stream, ELF_MAGIC, 4);
        WriteELF(stream, ELF_64);
        WriteELF(stream, ELF_LITTLE_ENDIAN);
        WriteELF(stream, ELF_EI_VERSION);
        WriteELF(stream, ELF_SYSV_ABI);
        WriteELF(stream, ELF_ABI_VERSION);
        WriteELF(stream, ELF_PAD, 7);

        WriteELF(stream, ELF_OBJECT);
        WriteELF(stream, ELF_AMD64);
        WriteELF(stream, ELF_VERSION);
        WriteELF(stream, ELF_ENTRY);
        WriteELF(stream, ELF_PHOFF);
        WriteELF(stream, ELF_SHOFF);
        WriteELF(stream, ELF_FLAGS);
        WriteELF(stream, ELF_EHSIZE);
        WriteELF(stream, ELF_PHSIZE);
        WriteELF(stream, ELF_PHNUM);
        WriteELF(stream, ELF_SHSIZE);
        WriteELF(stream, (short)mSections.size());
        WriteELF(stream, (short)1);

        ELFSection* shstrtab = getSection(".shstrtab");
        for (ELFSection& section : mSections)
        {
            section.mNameIdx = shstrtab->size();
            shstrtab->write(section.mName);
        }

        unsigned long currentOffset = ELF_SHSIZE * mSections.size() + ELF_EHSIZE;
        for (ELFSection& section : mSections)
        {
            currentOffset = (currentOffset + 15) & ~15; // Align to 16 bytes

            WriteELF(stream, section.mNameIdx);
            WriteELF(stream, section.mType);
            WriteELF(stream, section.mFlags);
            WriteELF(stream, SECTION_ADDR);
            WriteELF(stream, currentOffset);
            WriteELF(stream, (long)section.size());
            WriteELF(stream, section.mLink);
            WriteELF(stream, section.mInfo);
            WriteELF(stream, section.mAlign);
            WriteELF(stream, section.mEntrySize);

            currentOffset += section.size();
        }
        
        unsigned long written = 0;
        unsigned long alignedWritten = 0;
        for (ELFSection& section : mSections)
        {
            for (unsigned long i = 0; i < alignedWritten - written; i++)
            {
                stream.put(0);
            }
            written = alignedWritten;

            stream.write(section.mBuffer.view().data(), section.size());

            written += section.size();
            alignedWritten = (written + 15) & ~15;
        }
    }

    ELFFormat::ELFSection* ELFFormat::getSection(std::string const& name)
    {
        for (ELFSection& section : mSections)
        {
            if (section.mName == name)
            {
                return &section;
            }
        }

        return nullptr;
    }

    ELFFormat::ELFSection* ELFFormat::getSection(Section const section)
    {
        for (ELFSection& elfSection : mSections)
        {
            if (elfSection.mSection == section)
            {
                return &elfSection;
            }
        }

        return nullptr;
    }

    ELFFormat::ELFSection* ELFFormat::createSection(Section const section)
    {
        mSections.push_back(ELFSection(section));

        ELFSection* newSection = &mSections.back();

        ELFSection* strtab = getSection(".strtab");
        ELFSection* symtab = getSection(".symtab");

        symtab->write((unsigned int)strtab->mBuffer.view().length());
        symtab->write(SYM_SECTION);
        symtab->write(SYM_DEFAULT);
        symtab->write((unsigned short)(mSections.size() - 1));
        symtab->write((unsigned long)0);
        symtab->write((unsigned long)0);
        symtab->mInfo++;

        strtab->write(newSection->mName.data(), newSection->mName.length() + 1);

        return newSection;
    }

    ELFFormat::ELFSection* ELFFormat::getOrCreateSection(Section const section)
    {
        ELFSection* ret = getSection(section);
        if (!ret)
        {
            ret = createSection(section);
        }

        return ret;
    }
}