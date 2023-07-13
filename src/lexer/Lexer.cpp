#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <unordered_map>

namespace Lexing
{
    Lexer::Lexer(const std::string& text)
        :mText(text), mPosition(0)
    {
    }

    std::unordered_map<std::string_view, TokenType> keywords = {
        { "db", TokenType::DBInst },
        { "dw", TokenType::DWInst },
        { "dd", TokenType::DDInst },
        { "dq", TokenType::DQInst },
    };

    std::vector<Token> Lexer::lex()
    {
        std::vector<Token> tokens;

        while (mPosition < mText.length())
        {
            std::optional<Token> token = nextToken();
            if(token.has_value())
                tokens.push_back(*token);
            consume();
        }

        return tokens;
    }

    unsigned char Lexer::current()
    {
        return mText[mPosition];
    }

    unsigned char Lexer::consume()
    {
        return mText[mPosition++];
    }

    unsigned char Lexer::peek(int offset)
    {
        return mText[mPosition + offset];
    }

    std::optional<Token> Lexer::nextToken()
    {
        if (std::isalpha(current()) || current() == '_') // Identifier
        {
            std::string text = std::string(1, current());

            while (std::isalnum(peek(1)) || peek(1) == '_')
            {
                consume();
                text += current();
            }

            if(auto it = keywords.find(text); it != keywords.end())
                return Token(keywords.find(text)->second);

            return Token(TokenType::Identifier, std::move(text));
        }
        
        if (std::isspace(current())) // Newline, tab, space etc
        {
            return std::nullopt;
        }

        if (std::isdigit(current()))
        {
            std::string text = std::string(1, current());
            if (current() == '0')
            {
                if (peek(1) == 'x') // hex
                {
                    consume();
                    text += current();

                    while (std::isxdigit(peek(1)))
                    {
                        consume();
                        text += current();
                    }
                }
                else if (peek(1) == 'b') // binary
                {
                    consume();
                    text += current();

                    while (peek(1) == '0' || peek(1) == '1')
                    {
                        consume();
                        text += current();
                    }
                }
                else // octal
                {
                    consume();
                    text += current();

                    while (peek(1) >= '0' && peek(1) <= '7')
                    {
                        consume();
                        text += current();
                    }
                }
            }
            else
            {
                while (std::isdigit(peek(1)))
                {
                    consume();
                    text += current();
                }
            }
            return Token(TokenType::Immediate, std::move(text));
        }

        switch(current())
        {
            case '$':
            {
                if (peek(1) == '$')
                {
                    consume();
                    return Token(TokenType::DollarDollar);
                }
                return Token(TokenType::Dollar);
            }
            case '+':
                return Token(TokenType::Plus);
            case '-':
                return Token(TokenType::Minus);
            default:
                return Token(TokenType::Error); // Unknown character
        }
    }
}