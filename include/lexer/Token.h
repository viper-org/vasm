#ifndef VASM_LEXER_TOKEN_H
#define VASM_LEXER_TOKEN_H

#include <string>

namespace Lexing
{
    enum class TokenType : int
    {
        Error,

        Identifier,
        
        Immediate,

        DBInst, DWInst, DDInst, DQInst
    };

    class Token
    {
    public:
        Token(const TokenType tokenType, const std::string& text);
        Token(const TokenType tokenType, std::string&& text);
        Token(const TokenType tokenType);

        const char* getTokenTypeString() const;

        TokenType getTokenType() const;
        const std::string& getText() const;

        std::string toString() const;

        bool operator==(Token other);

    private:
        TokenType mTokenType{ TokenType::Error };

        std::string mText;
    };
}

#endif