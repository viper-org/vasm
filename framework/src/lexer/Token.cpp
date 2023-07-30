#include "lexer/Token.h"

namespace Lexing
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

    std::string to_string(const Lexing::TokenType& tokenType)
    {
        switch (tokenType)
        {
            case Lexing::TokenType::Error:
                return "Error";

            case Lexing::TokenType::Dollar:
                return "Dollar";
            case Lexing::TokenType::DollarDollar:
                return "DollarDollar";

            case Lexing::TokenType::Plus:
                return "Plus";
            case Lexing::TokenType::Minus:
                return "Minus";
            case Lexing::TokenType::Star:
                return "Star";
            case Lexing::TokenType::Slash:
                return "Slash";

            case Lexing::TokenType::LParen:
                return "LParen";
            case Lexing::TokenType::RParen:
                return "RParen";
            case Lexing::TokenType::LBracket:
                return "LBracket";
            case Lexing::TokenType::RBracket:
                return "RBracket";

            case Lexing::TokenType::Comma:
                return "Comma";
            case Lexing::TokenType::Colon:
                return "Colon";

            case Lexing::TokenType::Identifier:
                return "Identifier";

            case Lexing::TokenType::Immediate:
                return "Immediate";
            case Lexing::TokenType::String:
                return "String";
                
            case Lexing::TokenType::Register:
                return "Register";

            case Lexing::TokenType::Instruction:
                return "Instruction";
        }
        return "HELLO";
    }
}
