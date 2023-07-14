#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <array>
#include <unordered_map>

namespace Lexing
{
    Lexer::Lexer(const std::string& text)
        :mText(text), mPosition(0)
    {
    }

    std::vector<std::string_view> instructions = {
        "db",
        "dw",
        "dd",
        "dq",
        "jmp",
        "ret",
        "mov",
        "int",
        "times",
    };

    using namespace std::literals;
    
    constexpr std::array registers = {
        "al"sv, "ah"sv, "ax"sv, "eax"sv, "rax"sv,
        "bl"sv, "bh"sv, "bx"sv, "ebx"sv, "rbx"sv,
        "cl"sv, "ch"sv, "cx"sv, "ecx"sv, "rcx"sv,
        "dl"sv, "dh"sv, "dx"sv, "edx"sv, "rdx"sv,
        "spl"sv, "sp"sv, "esp"sv, "rsp"sv,
        "bpl"sv, "bp"sv, "ebp"sv, "rbp"sv,
        "sil"sv, "si"sv, "esi"sv, "rsi"sv,
        "dil"sv, "di"sv, "edi"sv, "rdi"sv,
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

            //if(auto it = keywords.find(text); it != keywords.end())
            //    return Token(keywords.find(text)->second);
            for (std::string_view instruction : instructions)
            {
                if (text == instruction)
                {
                    return Token(TokenType::Instruction, std::move(text));
                }
            }

            for (std::string_view reg : registers)
            {
                if (text == reg)
                {
                    return Token(TokenType::Register, std::move(text));
                }
            }

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
            
            case '(':
                return Token(TokenType::LParen);
            case ')':
                return Token(TokenType::RParen);

            case ',':
                return Token(TokenType::Comma);
            case ':':
                return Token(TokenType::Colon);
            
            default:
                return Token(TokenType::Error); // Unknown character
        }
    }
}