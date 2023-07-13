#include <parser/Parser.h>

#include <lexer/Token.h>

#include <codegen/OutputFormat.h>

#include <cstdint>
#include <iostream>
#include <format>
#include <string_view>

namespace Parsing
{
    Parser::Parser(std::vector<Lexing::Token>& tokens, Codegen::OutputFormat& output)
        :mTokens(tokens), mPosition(0), mOutput(output), mSection(Codegen::Section::Text)
    {
    }
    
    Lexing::Token Parser::current()
    {
        return mTokens[mPosition];
    }

    Lexing::Token Parser::consume()
    {
        return mTokens[mPosition++];
    }

    Lexing::Token Parser::peek(int offset)
    {
        return mTokens[mPosition + offset];
    }

    void Parser::expectToken(Lexing::TokenType tokenType)
    {
        if(current().getTokenType() != tokenType)
        {
            Lexing::Token temp(tokenType);
            std::cerr << std::format("Expected {}. Found {}. Terminating program.\n", temp.getTokenTypeString(), current().getText()); // TODO: Error properly
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
            
            case Lexing::TokenType::DBInst:
                parseDeclInst<char>();
                break;
            case Lexing::TokenType::DWInst:
                parseDeclInst<short>();
                break;
            case Lexing::TokenType::DDInst:
                parseDeclInst<int>();
                break;
            case Lexing::TokenType::DQInst:
                parseDeclInst<long>();
                break;

            case Lexing::TokenType::TimesStatement:
                parseTimesStatement();
                break;

            default:
                std::cerr << "Expected statement. Found " << current().getText() << ". Terminating program.\n"; // TODO: Error properly
                std::exit(1);
        }
    }

    template<typename T>
    void Parser::parseDeclInst()
    {
        consume();

        T value = parseExpression();
        mOutput.write(value, mSection);
    }

    void Parser::parseTimesStatement()
    {
        consume();

        long long iterations = parseExpression();

        int position = mPosition;
        while (--iterations)
        {
            parseStatement();
            mPosition = position;
        }
    }

    long long Parser::parseExpression(int precedence)
    {
        long long lhs = parseImmediate();
        while(mPosition < mTokens.size())
        {
            int binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType());
            if (binaryOperatorPrecedence < precedence)
                break;

            Lexing::TokenType operatorToken = consume().getTokenType();
            long long rhs = parseImmediate();
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

        if (current().getTokenType() == Lexing::TokenType::Immediate)
            return std::stoll(consume().getText(), 0, 0);

        else if (current().getTokenType() == Lexing::TokenType::Dollar)
        {
            consume();
            return mOutput.getPosition();
        }
        else if (current().getTokenType() == Lexing::TokenType::DollarDollar)
        {
            consume();
            return mOutput.getSectionStart(mSection);
        }
        
        else
            expectToken(Lexing::TokenType::Immediate);
        return -1;
    }
}