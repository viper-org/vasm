#include <lexer/Token.h>

#include <sstream>

namespace Lexing
{
    Token::Token(const TokenType tokenType, const std::string& text)
        :mTokenType(tokenType), mText(text)
    {
    }

    Token::Token(const TokenType tokenType, std::string&& text)
        :mTokenType(tokenType), mText(std::move(text))
    {
    }

    Token::Token(const TokenType tokenType)
        :mTokenType(tokenType), mText("")
    {
    }

    TokenType Token::getTokenType() const
    {
        return mTokenType;
    }

    const std::string& Token::getText() const
    {
        return mText;
    }

    const char* Token::getTokenTypeString() const
    {
        switch(mTokenType)
        {
            case TokenType::Error:
                return "Error";

            case TokenType::Dollar:
                return "Dollar";
            case TokenType::DollarDollar:
                return "DollarDollar";

            case TokenType::Plus:
                return "Plus";
            case TokenType::Minus:
                return "Minus";

            case TokenType::LParen:
                return "LParen";
            case TokenType::RParen:
                return "RParen";

            case TokenType::Identifier:
                return "Identifier";

            case TokenType::Immediate:
                return "Immediate";

            case TokenType::DBInst:
                return "DBInst";
            case TokenType::DWInst:
                return "DWInst";
            case TokenType::DDInst:
                return "DDInst";
            case TokenType::DQInst:
                return "DQInst";
        }
    }

    std::string Token::toString() const
    {
        std::stringstream ret;
        ret << getTokenTypeString() << "(" << mText << ")";
        return ret.str();
    }

    bool Token::operator==(Token other)
    {
        return ((mText == other.mText) && (mTokenType == other.mTokenType));
    }
}