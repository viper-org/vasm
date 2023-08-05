#include "lexer/Token.h"

namespace lexing
{
    SourceLocation::SourceLocation(size_t line, size_t column)
        : line(line)
        , column(column)
    {
    }

    Token::Token(SourceLocation sourceLocation, TokenType tokenType, std::string text)
        : mSourceLocation(sourceLocation)
        , mTokenType(tokenType)
        , mText(std::move(text))
    {
    }

    Token::Token(SourceLocation sourceLocation, TokenType tokenType)
        : mSourceLocation(sourceLocation)
        , mTokenType(tokenType)
        , mText()
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

    SourceLocation Token::getSourceLocation() const
    {
        return mSourceLocation;
    }

    bool Token::operator==(const Token& other) const
    {
        return (mText == other.mText) && (mTokenType == other.mTokenType);
    }
}
