#include <parser/Parser.h>

#include <lexer/Token.h>

#include <codegen/OutputFormat.h>
#include <codegen/Opcodes.h>

#include <iostream>

namespace Parsing
{
    constexpr unsigned char SIZE_16 = 0x66;

    Parser::Parser(std::vector<Lexing::Token>& tokens, Codegen::OutputFormat& output)
        :mTokens(tokens), mPosition(0), mOutput(output), mSection(Codegen::Section::Text)
    {
        mInstructionParsers = {
            {
                "db", [&](){
                    unsigned char value = parseExpression();
                    mOutput.write(value, mSection);
                }
            },
            {
                "dw", [&](){
                    unsigned short value = parseExpression();
                    mOutput.write(value, mSection);
                }
            },
            {
                "dd", [&](){
                    unsigned int value = parseExpression();
                    mOutput.write(value, mSection);
                }
            },
            {
                "dq", [&](){
                    unsigned long value = parseExpression();
                    mOutput.write(value, mSection);
                }
            },
            {
                "jmp", [&](){
                    unsigned char const value = parseExpression() - mOutput.getPosition(mSection) - 2; // Subtract size of the instruction itself

                    mOutput.write(Codegen::JMP_REL8, mSection);
                    mOutput.write(value, mSection);
                }
            },
            {
                "ret", [&](){
                    mOutput.write(Codegen::RET, mSection);
                }
            },
            {
                "mov", [&](){
                    std::pair<long long, Codegen::OperandSize> lhs = parseRegister();

                    expectToken(Lexing::TokenType::Comma);
                    consume();

                    if (isImmediate(current().getTokenType()))
                    {
                        long long immediate = parseExpression();
                        switch (lhs.second)
                        {
                            case Codegen::OperandSize::Byte:
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM8 + lhs.first), mSection);
                                mOutput.write((unsigned char)immediate, mSection);
                                break;
                            case Codegen::OperandSize::Word:
                                mOutput.write(SIZE_16, mSection);
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM + lhs.first), mSection);
                                mOutput.write((unsigned short)immediate, mSection);
                                break;
                            case Codegen::OperandSize::Long:
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM + lhs.first), mSection);
                                mOutput.write((unsigned int)immediate, mSection);
                                break;
                            case Codegen::OperandSize::Quad:
                                mOutput.write(Codegen::REX::W, mSection);
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM + lhs.first), mSection);
                                mOutput.write((unsigned long)immediate, mSection);
                                break;
                        }
                    }
                    else if (current().getTokenType() == Lexing::TokenType::Register)
                    {
                        std::pair<long long, Codegen::OperandSize> rhs = parseRegister();
                        if (lhs.second != rhs.second)
                        {
                            std::cerr << "Operand size mismatch on `mov' instruction. Terminating program.\n"; // TODO: Error properly
                            std::exit(1);
                        }

                        switch(lhs.second)
                        {
                            case Codegen::OperandSize::Byte:
                                mOutput.write(Codegen::MOV_REG_REG8, mSection);
                                break;
                            case Codegen::OperandSize::Word:
                                mOutput.write(SIZE_16, mSection);
                                mOutput.write(Codegen::MOV_REG_REG, mSection);
                                break;
                            case Codegen::OperandSize::Long:
                                mOutput.write(Codegen::MOV_REG_REG, mSection);
                                break;
                            case Codegen::OperandSize::Quad:
                                mOutput.write(Codegen::REX::W, mSection);
                                mOutput.write(Codegen::MOV_REG_REG, mSection);
                                break;
                        }
                        mOutput.write((unsigned char)(0xC0 + lhs.first + rhs.first * 8), mSection);
                    }
                }
            },
            {
                "int", [&](){
                    unsigned char vector = parseExpression();

                    mOutput.write(Codegen::INT, mSection);
                    mOutput.write(vector, mSection);
                }
            },
            {
                "times", [&](){
                    long long iterations = parseExpression();

                    int position = mPosition;
                    while (--iterations)
                    {
                        parseStatement();
                        mPosition = position;
                    }
                }
            },
        };
    }
    
    Lexing::Token& Parser::current()
    {
        return mTokens[mPosition];
    }

    Lexing::Token& Parser::consume()
    {
        return mTokens[mPosition++];
    }

    Lexing::Token& Parser::peek(int offset)
    {
        return mTokens[mPosition + offset];
    }

    void Parser::expectToken(Lexing::TokenType tokenType)
    {
        if(current().getTokenType() != tokenType)
        {
            Lexing::Token temp(tokenType);
            std::cerr << "Unexpected token. Terminating program.\n"; // TODO: Error properly
            std::exit(1);
        }
    }

    int Parser::getBinaryOperatorPrecedence(Lexing::TokenType tokenType) const
    {
        switch(tokenType)
        {
            case Lexing::TokenType::Plus:
            case Lexing::TokenType::Minus:
                return 35;
            
            default:
                return 0;
        }
    }

    bool Parser::isImmediate(Lexing::TokenType tokenType) const
    {
        return tokenType == Lexing::TokenType::Immediate || tokenType == Lexing::TokenType::Identifier || tokenType == Lexing::TokenType::Dollar || tokenType == Lexing::TokenType::DollarDollar;
    }

    void Parser::parse()
    {
        while (mPosition < mTokens.size())
        {
            parseStatement();
        }
    }

    void Parser::parseStatement()
    {
        switch(current().getTokenType())
        {
            case Lexing::TokenType::Error:
                std::cerr << "Found unknown symbol. Terminating program.\n"; // TODO: Error properly
                std::exit(1);

            case Lexing::TokenType::Identifier:
                parseLabel();
                break;

            case Lexing::TokenType::Instruction:
            {
                consume();
                InstructionParser parser = mInstructionParsers.at(current().getText());
                parser();
                break;
            }

            default:
                std::cerr << "Expected statement. Found " << current().getText() << ". Terminating program.\n"; // TODO: Error properly
                std::exit(1);
        }
    }

    void Parser::parseLabel()
    {
        const std::string& name = consume().getText();

        expectToken(Lexing::TokenType::Colon);
        consume();

        mOutput.addSymbol(name, mOutput.getPosition(mSection), mSection, Codegen::Global(true)); // TODO: Check if global
    }

    long long Parser::parseExpression(int precedence)
    {
        long long lhs = parseImmediate();
        int binaryOperatorPrecedence;
        while (binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType()), binaryOperatorPrecedence > precedence)
        {
            Lexing::TokenType operatorToken = consume().getTokenType();

            long long rhs = parseExpression(binaryOperatorPrecedence);

            switch(operatorToken)
            {
                case Lexing::TokenType::Plus:
                    lhs += rhs;
                    break;
                case Lexing::TokenType::Minus:
                    lhs -= rhs;
                    break;
                default:
                    break;
            }
        }
        return lhs;
    }

    long long Parser::parseImmediate()
    {
        if(current().getTokenType() == Lexing::TokenType::LParen)
        {
            consume();

            long long ret = parseExpression();

            expectToken(Lexing::TokenType::RParen);
            consume();

            return ret;
        }
        else if (current().getTokenType() == Lexing::TokenType::Immediate)
        {
            return std::stoll(consume().getText(), 0, 0);
        }
        else if (current().getTokenType() == Lexing::TokenType::Dollar)
        {
            consume();
            return mOutput.getPosition(mSection);
        }
        else if (current().getTokenType() == Lexing::TokenType::DollarDollar)
        {
            consume();
            return mOutput.getSectionStart(mSection);
        }
        else if (current().getTokenType() == Lexing::TokenType::Identifier)
        {
            return mOutput.getSymbol(consume().getText());
        }
        else
            expectToken(Lexing::TokenType::Immediate); // TODO: Error properly

        return -1;
    }

    std::pair<long long, Codegen::OperandSize> Parser::parseRegister()
    {
        constexpr int REGISTERS_PER_ENCODING = 4;

        long long index;
        for (index = 0; index < Codegen::Registers.size(); index++)
        {
            if (Codegen::Registers[index] == current().getText())
            {
                break;
            }
        }
        consume();

        return std::make_pair(index / REGISTERS_PER_ENCODING, static_cast<Codegen::OperandSize>(index % REGISTERS_PER_ENCODING));
    }
}