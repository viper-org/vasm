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
    
    struct SourceLocation {
        SourceLocation(size_t line, size_t column);
        
        size_t line;
        size_t column;
    };

    class Token
    {
    public:
        Token(SourceLocation location, TokenType tokenType, std::string text);
        Token(SourceLocation location, TokenType tokenType);


        [[nodiscard]] TokenType getTokenType() const;
        [[nodiscard]] const std::string& getText() const;
        [[nodiscard]] SourceLocation getSourceLocation() const;

        bool operator==(const Token& other) const;
        
    private:
        TokenType mTokenType{ TokenType::Error };

        std::string mText;

        SourceLocation mSourceLocation;
    };

    std::string to_string(const Lexing::TokenType& tokenType);
}


#endif