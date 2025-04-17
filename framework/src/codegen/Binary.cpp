#include "vasm/codegen/Binary.h"

namespace codegen
{
    BinaryFormat::BinaryFormat() 
        : mBuffer{}
    {}

    void BinaryFormat::write(std::uint8_t data, std::string, std::uint64_t offset)
    {
        if (offset != -1)
            mBuffer.insert(mBuffer.begin() + offset, data);
        else
            mBuffer.push_back(data);
    }

    void BinaryFormat::write(std::uint16_t data, std::string, std::uint64_t offset)
    {
        write((std::uint8_t)data, "", offset);
        write((std::uint8_t)(data >> 8), "", offset + 1);
    }

    void BinaryFormat::write(std::uint32_t data, std::string, std::uint64_t offset)
    {
        write((std::uint8_t)data, "", offset);
        write((std::uint8_t)(data >> 8), "", offset + 1);
        write((std::uint8_t)(data >> 16), "", offset + 2);
        write((std::uint8_t)(data >> 24), "", offset + 3);
    }

    void BinaryFormat::write(std::uint64_t data, std::string, std::uint64_t offset)
    {
        write((std::uint8_t)data, "", offset);
        write((std::uint8_t)(data >> 8), "", offset + 1);
        write((std::uint8_t)(data >> 16), "", offset + 2);
        write((std::uint8_t)(data >> 24), "", offset + 3);
        write((std::uint8_t)(data >> 32), "", offset + 4);
        write((std::uint8_t)(data >> 40), "", offset + 5);
        write((std::uint8_t)(data >> 48), "", offset + 6);
        write((std::uint8_t)(data >> 56), "", offset + 7);
    }


    size_t BinaryFormat::getPosition(std::string)
    {
        return mBuffer.size();
    }

    size_t BinaryFormat::getSectionStart(std::string)
    {
        return 0;
    }

    void BinaryFormat::addSymbol(const std::string& name, std::uint64_t value, std::string const, bool)
    {
        mSymbols[name] = value;
    }

    void BinaryFormat::addExternSymbol(const std::string&)
    {
        // TODO: Error
    }

    std::pair<std::uint64_t, bool> BinaryFormat::getSymbol(const std::string& name) const
    {
        if (mSymbols.find(name) != mSymbols.end())
        {
            return std::make_pair(mSymbols.at(name), false);
        }
        return std::make_pair(-1, false); // Forward symbol
    }

    std::string BinaryFormat::getSymbolAfter(const std::string& name) const
    { // TODO: Implement
    }

    void BinaryFormat::createSection(SectionInfo* info)
    { // TODO: Error
    }

    std::string BinaryFormat::getSymbolSection(std::string_view name) const
    { // TODO: Implement
    }
    std::string BinaryFormat::getSection(std::string_view name)
    { // TODO: Implement
    }

    std::string BinaryFormat::getCodeSectionName()
    {
        return "";
    }
    
    bool BinaryFormat::hasSymbol(const std::string& name) const
    {
        return mSymbols.contains(name);
    }

    void BinaryFormat::relocSymbol(const std::string&, const std::string&, std::string, int, int)
    {
        // TODO: Error
    }

    void BinaryFormat::patchForwardSymbol(const std::string& name, std::string section, OperandSize size, int location, int origin)
    {
        std::uint64_t symbol = getSymbol(name).first - origin;
        switch (size)
        {
            case OperandSize::Byte:
                mBuffer[location] = symbol;
                break;
            case OperandSize::Word:
                mBuffer[location] = symbol;
                mBuffer[location + 1] = symbol >> 8;
                break;
            case OperandSize::Long:
                mBuffer[location] = symbol;
                mBuffer[location + 1] = symbol >> 8;
                mBuffer[location + 2] = symbol >> 16;
                mBuffer[location + 3] = symbol >> 24;
                break;
            case OperandSize::Quad:
                mBuffer[location] = symbol;
                mBuffer[location + 1] = symbol >> 8;
                mBuffer[location + 2] = symbol >> 16;
                mBuffer[location + 3] = symbol >> 24;
                mBuffer[location + 4] = symbol >> 32;
                mBuffer[location + 5] = symbol >> 40;
                mBuffer[location + 6] = symbol >> 48;
                mBuffer[location + 7] = symbol >> 56;
                break;
            case OperandSize::None:
                break;
        }
    }

    void BinaryFormat::print(std::ostream& stream)
    {
        stream.write(reinterpret_cast<const char*>(mBuffer.data()), static_cast<std::streamsize>(mBuffer.size()));
    }

    std::unique_ptr<unsigned char[]> BinaryFormat::loadjit()
    {
        std::unique_ptr<unsigned char[]> ret = std::make_unique<unsigned char[]>(mBuffer.size());
        std::copy(mBuffer.begin(), mBuffer.end(), ret.get());
        return ret;
    }
}