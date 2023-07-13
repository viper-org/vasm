#include <codegen/Binary.h>
#include <lexer/Token.h>

namespace Codegen
{
    BinaryFormat::BinaryFormat()
        :mBuffer(std::ios::out | std::ios::binary)
    {
    }


    void BinaryFormat::write(char const data, Section const)
    {
        mBuffer.write((char*)&data, sizeof(data));
    }

    void BinaryFormat::write(short const data, Section const)
    {
        mBuffer.write((char*)&data, sizeof(data));
    }

    void BinaryFormat::write(int const data, Section const)
    {
        mBuffer.write((char*)&data, sizeof(data));
    }

    void BinaryFormat::write(long const data, Section const)
    {
        mBuffer.write((char*)&data, sizeof(data));
    }


    int BinaryFormat::getPosition()
    {
        return mBuffer.view().length();
    }

    int BinaryFormat::getSectionStart(Section const)
    {
        return 0;
    }

    void BinaryFormat::print(std::ostream& stream)
    {
        stream << mBuffer.rdbuf()->view();
    }
}