#ifndef VASM_LEXER_LEXER_H
#define VASM_LEXER_LEXER_H

#include <optional>
#include <string>
#include <vector>

namespace Lexing
{
    class Token;

    class Lexer
    {
    public:
        Lexer(const std::string& text);

        std::vector<Token> lex();
    private:
        const std::string& mText;

        int mPosition{ 0 };

        unsigned char current();
        unsigned char consume();
        unsigned char peek(int offset);

        std::optional<Token> nextToken();
    };
}

#endif