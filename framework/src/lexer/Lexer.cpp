#include "vasm/lexer/Lexer.h"
#include "vasm/lexer/Token.h"

#include "vasm/codegen/Opcodes.h"

#include <unordered_map>

namespace lexing
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
        "ja",
        "jae",
        "jb",
        "jbe",
        "jc",
        "jcxz",
        "jecxz",
        "jrcxz",
        "je",
        "jg",
        "jge",
        "jl",
        "jle",
        "jna",
        "jnae",
        "jnb",
        "jnbe",
        "jnc",
        "jne",
        "jng",
        "jnge",
        "jnl",
        "jnle",
        "jno",
        "jnp",
        "jns",
        "jnz",
        "jo",
        "jp",
        "jpe",
        "jpo",
        "js",
        "jz",

        "call",

        "ret",

        "leave",

        "mov",
        "movzx",
        "movsx",
        "lea",

        "rol",
        "ror",
        "rcl",
        "rcr",
        "shl",
        "shr",
        "sal",
        "sar",

        "xchg",

        "add",
        "adc",
        "sbb",
        "or",
        "and",
        "sub",
        "xor",
        "cmp",

        "test",
        "not",
        "neg",
        "mul",
        "imul",
        "div",
        "idiv",

        "push",
        "pop",

        "inc",
        "dec",

        "in",
        "out",

        "int",
        "syscall",

        "nop",

        "times",
    };

    const std::unordered_map<std::string_view, lexing::TokenType> directives = {
        {"extern", lexing::TokenType::Extern}
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
        tokens.emplace_back(SourceLocation(line, column), lexing::TokenType::End);

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
        SourceLocation startSourceLocation {line, column};
        
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
                    return Token {startSourceLocation, TokenType::Instruction, std::move(text)};
                }
            }

            for (std::string_view reg : codegen::Registers)
            {
                if (text == reg)
                {
                    return Token {startSourceLocation, TokenType::Register, std::move(text)};
                }
            }

            if (auto it = directives.find(text); it != directives.end())
            {
                return Token {startSourceLocation, it->second};
            }

            if (text == "byte" || text == "word" || text == "long" || text == "quad")
            {
                return Token {startSourceLocation, TokenType::Size, text};
            }

            return Token {startSourceLocation, TokenType::Identifier, std::move(text)};
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
            return Token {startSourceLocation, TokenType::Immediate, std::move(text)};
        }

        switch(current())
        {
            case '$':
            {
                if (peek(1) == '$')
                {
                    consume();
                    return Token {startSourceLocation, TokenType::DollarDollar};
                }
                return Token {startSourceLocation, TokenType::Dollar};
            }

            case '+':
                return Token {startSourceLocation, TokenType::Plus};
            case '-':
                return Token {startSourceLocation, TokenType::Minus};
            case '*':
                return Token {startSourceLocation, TokenType::Star};
            case '/':
                return Token {startSourceLocation, TokenType::Slash};
            
            case '(':
                return Token {startSourceLocation, TokenType::LParen};
            case ')':
                return Token {startSourceLocation, TokenType::RParen};
            case '[':
                return Token {startSourceLocation, TokenType::LBracket};
            case ']':
                return Token {startSourceLocation, TokenType::RBracket};

            case ',':
                return Token {startSourceLocation, TokenType::Comma};
            case ':':
                return Token {startSourceLocation, TokenType::Colon};

            case '@':
                return Token {startSourceLocation, TokenType::Ampersand};

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
                                    return Token {startSourceLocation, TokenType::Error};
                            }
                            break;
                        }
                        default:
                            value += current();
                    }
                    consume();
                }
                return Token {startSourceLocation, TokenType::String, std::move(value)};
            }
            
            default:
                return Token {startSourceLocation, TokenType::Error}; // Unknown character
        }
    }
}
