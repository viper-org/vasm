#ifndef VASM_LEXER_TOKEN_H
#define VASM_LEXER_TOKEN_H

#include <string>

namespace Lexing
{
    enum class TokenType : int
    {
        Error,

        Dollar, DollarDollar,

        Plus, Minus, Star, Slash,
        LParen, RParen, LBracket, RBracket,
        Comma, Colon,

        Identifier,
        
        Immediate, String,

        Register,

        Instruction,
    };
    
    struct SrcLocation {
        SrcLocation(size_t line, size_t column) : line {line}, column {column} {}
        
        size_t line;
        size_t column;
    };

    class Token
    {
    public:
        Token(SrcLocation location, TokenType tokenType, std::string text);
        explicit Token(SrcLocation location, TokenType tokenType);

        [[nodiscard]] TokenType getTokenType() const;
        [[nodiscard]] const std::string& getText() const;

        bool operator==(const Token& other) const;
        
        const SrcLocation location;
    private:
        TokenType mTokenType{ TokenType::Error };

        std::string mText;
    };
}

#endif