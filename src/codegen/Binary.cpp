#include <codegen/Binary.h>
#include <iostream>
#include <lexer/Token.h>

namespace Codegen
{
    BinaryFormat::BinaryFormat()
        :mBuffer(std::ios::out | std::ios::binary)
    {
    }


    void BinaryFormat::write(unsigned char const data, Section const)
    {
        mBuffer.write((char const*)&data, sizeof(data));
    }

    void BinaryFormat::write(unsigned short const data, Section const)
    {
        mBuffer.write((char const*)&data, sizeof(data));
    }

    void BinaryFormat::write(unsigned int const data, Section const)
    {
        mBuffer.write((char const*)&data, sizeof(data));
    }

    void BinaryFormat::write(unsigned long const data, Section const)
    {
        mBuffer.write((char const*)&data, sizeof(data));
    }


    int BinaryFormat::getPosition(Section const)
    {
        return mBuffer.view().length();
    }

    int BinaryFormat::getSectionStart(Section const)
    {
        return 0;
    }

    void BinaryFormat::addSymbol(const std::string& name, unsigned long value, Section const, bool)
    {
        mSymbols[name] = value;
    }

    unsigned long BinaryFormat::getSymbol(const std::string& name)
    {
        return mSymbols.at(name);
    }

    void BinaryFormat::print(std::ostream& stream)
    {
        stream.write(mBuffer.view().data(), mBuffer.view().length());
    }
}