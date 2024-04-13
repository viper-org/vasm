#include "codegen/FakeOutputFormat.h"

#include <algorithm>

namespace codegen
{
    namespace fakes
    {
        FakeOutputFormat::FakeOutputFormat()
            : mCode()
            , mData()
        {
        }

        void FakeOutputFormat::write(std::uint8_t data, Section section)
        {
            auto& buffer = (section == Section::Text) ? mCode : mData;
            buffer.push_back(data);
        }

        void FakeOutputFormat::write(std::uint16_t data, Section section)
        {
            auto& buffer = (section == Section::Text) ? mCode : mData;
            buffer.push_back(data);
            buffer.push_back(data >> 8);
        }

        void FakeOutputFormat::write(std::uint32_t data, Section section)
        {
            auto& buffer = (section == Section::Text) ? mCode : mData;
            buffer.push_back(data);
            buffer.push_back(data >> 8);
            buffer.push_back(data >> 16);
            buffer.push_back(data >> 24);
        }

        void FakeOutputFormat::write(std::uint64_t data, Section section)
        {
            auto& buffer = (section == Section::Text) ? mCode : mData;
            buffer.push_back(data);
            buffer.push_back(data >> 8);
            buffer.push_back(data >> 16);
            buffer.push_back(data >> 24);
            buffer.push_back(data >> 32);
            buffer.push_back(data >> 40);
            buffer.push_back(data >> 48);
            buffer.push_back(data >> 56);
        }

        size_t FakeOutputFormat::getPosition(Section section)
        {
            const auto& buffer = (section == Section::Text) ? mCode : mData;

            return buffer.size();
        }

        size_t FakeOutputFormat::getSectionStart(Section section)
        {
            return 0;
        }

        void FakeOutputFormat::addSymbol(const std::string& name, unsigned long value, Section section, bool isGlobal)
        {
            mSymbols.push_back({name, value, section, isGlobal});
        }

        void FakeOutputFormat::addExternSymbol(const std::string&)
        {
            // TODO: Implement
        }

        std::pair<unsigned long, bool> FakeOutputFormat::getSymbol(const std::string& name) const
        {
            auto it = std::find_if(mSymbols.begin(), mSymbols.end(), [&name](auto& symbol) {
                return symbol.name == name;
            });

            if (it == mSymbols.end())
            {
                return std::make_pair(0, false);
            }

            return std::make_pair(it->value, false);
        }

        bool FakeOutputFormat::hasSymbol(const std::string& name) const
        {
            auto it = std::find_if(mSymbols.begin(), mSymbols.end(), [&name](auto& symbol) {
                return symbol.name == name;
            });

            return (it != mSymbols.end());
        }

        void FakeOutputFormat::relocSymbol(const std::string& name, const std::string& location, Section section, int)
        {
            mRelocations.push_back({ name, section });
        }

        void FakeOutputFormat::patchForwardSymbol(const std::string& name, Section section, OperandSize size, int location, int origin)
        {
        }

        void FakeOutputFormat::print(std::ostream&)
        {
        }

        const std::vector<unsigned char>& FakeOutputFormat::getCode() const
        {
            return mCode;
        }

        const std::vector<unsigned char>& FakeOutputFormat::getData() const
        {
            return mData;
        }

        const std::vector<FakeSymbol>& FakeOutputFormat::getSymbols() const
        {
            return mSymbols;
        }

        const std::vector<FakeRelocation>& FakeOutputFormat::getRelocations() const
        {
            return mRelocations;
        }
    }
}