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
        explicit Lexer(std::string_view text);

        std::vector<Token> lex();
    private:
        std::string_view mText;

        size_t mPosition {0};
		size_t line {1};
		size_t column {1};

        [[nodiscard]] char current() const;
        char consume();
        [[nodiscard]] char peek(size_t offset) const;

        std::optional<Token> nextToken();
    };
}

#endif