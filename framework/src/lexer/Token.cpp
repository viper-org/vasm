#include "lexer/Token.h"

namespace Lexing
{
    SourceLocation::SourceLocation(size_t line, size_t column)
        : line(line)
        , column(column)
    {
    }

    Token::Token(SourceLocation location, TokenType tokenType, std::string text)
        : location(location)
        , mTokenType(tokenType)
        , mText(std::move(text))
    {
    }

    Token::Token(SourceLocation location, TokenType tokenType)
        : location {location}, mTokenType {tokenType}
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

    bool Token::operator==(const Token& other) const
    {
        return (mText == other.mText) && (mTokenType == other.mTokenType);
    }
}