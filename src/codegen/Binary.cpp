#include <codegen/Binary.h>

namespace Codegen
{
    BinaryFormat::BinaryFormat() : mBuffer {} {}

    void BinaryFormat::write(unsigned char data, Section)
    {
        mBuffer.push_back(data);
    }

    void BinaryFormat::write(unsigned short data, Section)
    {
        mBuffer.push_back(data);
        mBuffer.push_back(data >> 8);
    }

    void BinaryFormat::write(unsigned int data, Section)
    {
        mBuffer.push_back(data);
        mBuffer.push_back(data >> 8);
        mBuffer.push_back(data >> 16);
        mBuffer.push_back(data >> 24);
    }

    void BinaryFormat::write(unsigned long data, Section)
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

    void BinaryFormat::addSymbol(const std::string& name, unsigned long value, Section const, bool)
    {
        mSymbols[name] = value;
    }

    unsigned long BinaryFormat::getSymbol(const std::string& name) const
    {
        return mSymbols.at(name);
    }
    
    bool BinaryFormat::hasSymbol(const std::string& name) const {
        return mSymbols.contains(name);
    }

    void BinaryFormat::relocSymbol(const std::string&, Section const)
    {
    }

    void BinaryFormat::print(std::ostream& stream)
    {
        stream.write(reinterpret_cast<const char*>(mBuffer.data()), static_cast<std::streamsize>(mBuffer.size()));
    }
}