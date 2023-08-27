#include "vasm/codegen/Binary.h"

namespace codegen
{
    BinaryFormat::BinaryFormat() 
        : mBuffer{}
    {}

    void BinaryFormat::write(std::uint8_t data, Section)
    {
        mBuffer.push_back(data);
    }

    void BinaryFormat::write(std::uint16_t data, Section)
    {
        mBuffer.push_back(data);
        mBuffer.push_back(data >> 8);
    }

    void BinaryFormat::write(std::uint32_t data, Section)
    {
        mBuffer.push_back(data);
        mBuffer.push_back(data >> 8);
        mBuffer.push_back(data >> 16);
        mBuffer.push_back(data >> 24);
    }

    void BinaryFormat::write(std::uint64_t data, Section)
    {
        mBuffer.push_back(data);
        mBuffer.push_back(data >> 8);
        mBuffer.push_back(data >> 16);
        mBuffer.push_back(data >> 24);
        mBuffer.push_back(data >> 32);
        mBuffer.push_back(data >> 40);
        mBuffer.push_back(data >> 48);
        mBuffer.push_back(data >> 56);
    }


    size_t BinaryFormat::getPosition(Section)
    {
        return mBuffer.size();
    }

    size_t BinaryFormat::getSectionStart(Section)
    {
        return 0;
    }

    void BinaryFormat::addSymbol(const std::string& name,  std::uint64_t value, Section const, bool)
    {
        mSymbols[name] = value;
    }

    void BinaryFormat::addExternSymbol(const std::string&)
    {
        // TODO: Error
    }

    std::pair< std::uint64_t, bool> BinaryFormat::getSymbol(const std::string& name) const
    {
        if (mSymbols.find(name) != mSymbols.end())
        {
            return std::make_pair(mSymbols.at(name), false);
        }
        return std::make_pair(-1, false); // Forward symbol
    }
    
    bool BinaryFormat::hasSymbol(const std::string& name) const {
        return mSymbols.contains(name);
    }

    void BinaryFormat::relocSymbol(const std::string&, const std::string&, Section, int)
    {
        // TODO: Error
    }

    void BinaryFormat::patchForwardSymbol(const std::string& name, Section section, OperandSize size, int location, int origin)
    {
        // TODO: Implement
    }

    void BinaryFormat::print(std::ostream& stream)
    {
        stream.write(reinterpret_cast<const char*>(mBuffer.data()), static_cast<std::streamsize>(mBuffer.size()));
    }
}