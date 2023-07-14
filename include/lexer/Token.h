#ifndef VASM_LEXER_TOKEN_H
#define VASM_LEXER_TOKEN_H

#include <string>

namespace Lexing
{
    enum class TokenType : int
    {
        Error,

        Dollar, DollarDollar,

        Plus, Minus,
        LParen, RParen,
        Comma, Colon,

        Identifier,
        
        Immediate,

        Register,

        Instruction,

        DBInst, DWInst, DDInst, DQInst,

        JumpInst,
        RetInst,

        MovInst,

        IntInst,

        TimesStatement,
    };

    class Token
    {
    public:
        Token(const TokenType tokenType, const std::string& text);
        Token(const TokenType tokenType, std::string&& text);
        Token(const TokenType tokenType);

        TokenType getTokenType() const;
        const std::string& getText() const;

        bool operator==(Token other);

    private:
        TokenType mTokenType{ TokenType::Error };

        std::string mText;
    };
}

#endif