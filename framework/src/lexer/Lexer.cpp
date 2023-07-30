#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include <codegen/Opcodes.h>
#include <iostream>

namespace Lexing
{
    Lexer::Lexer(std::string_view text)
        : mText(text)
    {
    }

    constexpr std::array instructions = {
        "db",
        "dw",
        "dd",
        "dq",
        "jmp",
        "call",
        "ret",
        "mov",
        "lea",
        "int",
        "syscall",
        "times",
    };

    std::vector<Token> Lexer::lex()
    {
        std::vector<Token> tokens;

        while (mPosition < mText.length())
        {
            std::optional<Token> token = nextToken();
            if (token.has_value())
            {
                tokens.push_back(*token);
            }
            consume();
        }

        return tokens;
    }

    char Lexer::current() const
    {
        return mText[mPosition];
    }

    char Lexer::consume()
    {
        char c = mText[mPosition++];
        if (c == '\n') {
			column = 1;
			line += 1;
        }
        else {
			column += 1;
        }
        return c;
    }

    char Lexer::peek(size_t offset) const
    {
        return mText[mPosition + offset];
    }

    std::optional<Token> Lexer::nextToken()
    {
        SourceLocation start_loc {line, column};
        
        if (std::isalpha(current()) || current() == '_') // Identifier
        {
            std::string text = std::string(1, current());

            while (std::isalnum(peek(1)) || peek(1) == '_')
            {
                consume();
                text += current();
            }

            for (std::string_view instruction : instructions)
            {
                if (text == instruction)
                {
                    return Token {start_loc, TokenType::Instruction, std::move(text)};
                }
            }

            for (std::string_view reg : Codegen::Registers)
            {
                if (text == reg)
                {
                    return Token {start_loc, TokenType::Register, std::move(text)};
                }
            }

            return Token {start_loc, TokenType::Identifier, std::move(text)};
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
                    while (peek(1) >= '0' && peek(1) <= '7')
                    {
                        consume();
                        text += current();
                    }
                }
            }
            else // decimal
            {
                while (std::isdigit(peek(1)))
                {
                    consume();
                    text += current();
                }
            }
            return Token {start_loc, TokenType::Immediate, std::move(text)};
        }

        switch(current())
        {
            case '$':
            {
                if (peek(1) == '$')
                {
                    consume();
                    return Token {start_loc, TokenType::DollarDollar};
                }
                return Token {start_loc, TokenType::Dollar};
            }

            case '+':
                return Token {start_loc, TokenType::Plus};
            case '-':
                return Token {start_loc, TokenType::Minus};
            case '*':
                return Token {start_loc, TokenType::Star};
            case '/':
                return Token {start_loc, TokenType::Slash};
            
            case '(':
                return Token {start_loc, TokenType::LParen};
            case ')':
                return Token {start_loc, TokenType::RParen};
            case '[':
                return Token {start_loc, TokenType::LBracket};
            case ']':
                return Token {start_loc, TokenType::RBracket};

            case ',':
                return Token {start_loc, TokenType::Comma};
            case ':':
                return Token {start_loc, TokenType::Colon};

            case '"':
            {
                consume();
                std::string value;
                while(current() != '"')
                {
                    switch(current())
                    {
                        case '\\':
                        {
                            consume();
                            switch(current())
                            {
                                case 'n':
                                    value += '\n';
                                    break;
                                case '\'':
                                    value += '\'';
                                    break;
                                case '\\':
                                    value += '\\';
                                    break;
                                case '0':
                                    value += '\0';
                                    break;
                                default:
                                    std::cout << current() << "\n";
                                    return Token {start_loc, TokenType::Error};
                            }
                            break;
                        }
                        default:
                            value += current();
                    }
                    consume();
                }
                return Token {start_loc, TokenType::String, std::move(value)};
            }
            
            default:
                return Token {start_loc, TokenType::Error}; // Unknown character
        }
    }
}
