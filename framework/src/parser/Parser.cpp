#include "parser/Parser.h"

#include "lexer/Token.h"

#include "codegen/IOutputFormat.h"
#include "codegen/Opcodes.h"

#include <cstdint>
#include <iostream>

namespace parsing
{
    constexpr unsigned char SIZE_16 = 0x66;

    Parser::Parser(std::string_view filename, std::vector<lexing::Token>& tokens, codegen::IOutputFormat& output)
        : filename {filename}
        , mTokens {tokens}
        , mOutput {output}
        , mPosition {0}
        , mSection {codegen::Section::Text}
    {
        mInstructionParsers = {
            {
                "db", [&](){
                    if (current().getTokenType() == lexing::TokenType::String)
                    {
                        for (unsigned char character : consume().getText())
                        {
                            mOutput.write(character, mSection);
                        }
                    }
                    else
                    {
                        unsigned char value = parseExpression();
                        mOutput.write(value, mSection);
                    }
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
                    unsigned char value = parseExpression() - mOutput.getPosition(mSection) - 2; // Subtract size of the instruction itself

                    mOutput.write(codegen::JMP_REL8, mSection);
                    mOutput.write(value, mSection);
                }
            },
            {
                "call", [&](){
                    unsigned int value = parseExpression() - mOutput.getPosition(mSection) - 5; // Subtract size of the instruction itself

                    mOutput.write(codegen::CALL_REL32, mSection);
                    mOutput.write(value, mSection);
                }
            },
            {
                "ret", [&](){
                    mOutput.write(codegen::RET, mSection);
                }
            },
            {
                "lea", [&](){
                    auto registerToken = current();
                    std::pair<long long, codegen::OperandSize> lhs = parseRegister();
                    
                    expectToken(lexing::TokenType::Comma, "Expected ',' after lea destination register.");
                    consume();
                    
                    expectToken(lexing::TokenType::LBracket, "Expected '[' before lea source expression.");
                    auto bracketStartToken = consume();
                    
                    // TODO: Maybe a proper keyword in lexer?
                    bool rel = false;
                    if (current().getText() == "rel") {
                        consume();
                        rel = true;
                    }
                    
                    expectToken(lexing::TokenType::Identifier, "Expected an identifier in lea source expression.");
                    long long labelAddr = parseImmediate();
                    
                    expectToken(lexing::TokenType::RBracket, "Expected ']' after lea source expression.");
                    consume();
                    
                    if (!rel) {
                        reportError(bracketStartToken, "Not relative lea instruction is not supported.");
                    }
                    
                    int extraBytes = 0;
                    auto start = mOutput.getPosition(mSection);
                    switch (lhs.second) {
                        case codegen::OperandSize::Byte:
                            reportError(registerToken, "Unsupported operand size for 'lea' instruction.");
                            break;
                        case codegen::OperandSize::Quad:
                            mOutput.write(codegen::REX::W, mSection);
                            extraBytes = 1;
                            break;
                        case codegen::OperandSize::Word:
                            mOutput.write(codegen::SIZE_PREFIX, mSection);
                            extraBytes = 1;
                            break;
                        default:
                            break;
                    }

                    mOutput.write(codegen::LEA, mSection);
                    if (rel)
                    {
                        // mod = 0b11000000
                        // reg = 0b00111000
                        // rm  = 0b00000111
                        
                        // rm none | mod none | reg
                        
                        // TODO: Extended registers
                        uint8_t modrm = 0b101 | 0b00 << 6 | (lhs.first & 0b111) << 3;
                        mOutput.write(modrm, mSection);
                        // This is actually signed but to avoid casting just use unsigned int
                        unsigned int value = labelAddr - start - (6 + extraBytes);
                        mOutput.write(value, mSection);
                    }
                }
            },
            {
                "mov", [&](){
                    std::pair<long long, codegen::OperandSize> lhs = parseRegister();

                    expectToken(lexing::TokenType::Comma, "Expected ',' after mov destination register.");
                    consume();

                    if (isImmediate(current().getTokenType()))
                    {
                        lexing::Token* reloc = nullptr;
                        if (current().getTokenType() == lexing::TokenType::Identifier) // Emit a relocation for symbols
                        {
                            reloc = &current();
                        }

                        long long immediate = parseExpression();
                        switch (lhs.second)
                        {
                            case codegen::OperandSize::Byte:
                                mOutput.write((unsigned char)(codegen::MOV_REG_IMM8 + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                    mOutput.write((unsigned char) 0, mSection);
                                }
                                else
                                {
                                    mOutput.write((unsigned char)immediate, mSection);
                                }
                                break;
                            case codegen::OperandSize::Word:
                                mOutput.write(SIZE_16, mSection);
                                mOutput.write((unsigned char)(codegen::MOV_REG_IMM + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                    mOutput.write((unsigned short) 0, mSection);
                                }
                                else
                                {
                                    mOutput.write((unsigned short)immediate, mSection);
                                }
                                break;
                            case codegen::OperandSize::Long:
                                mOutput.write((unsigned char)(codegen::MOV_REG_IMM + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                    mOutput.write((unsigned int) 0, mSection);
                                }
                                else
                                {
                                    mOutput.write((unsigned int)immediate, mSection);
                                }
                                break;
                            case codegen::OperandSize::Quad:
                                mOutput.write(codegen::REX::W, mSection);
                                mOutput.write((unsigned char)(codegen::MOV_REG_IMM + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                    mOutput.write((unsigned long) 0, mSection);
                                }
                                else
                                {
                                    mOutput.write((unsigned long)immediate, mSection);
                                }
                                break;
                        }
                    }
                    else if (current().getTokenType() == lexing::TokenType::Register)
                    {
                        auto rhsRegisterToken = current();
                        std::pair<long long, codegen::OperandSize> rhs = parseRegister();
                        if (lhs.second != rhs.second)
                        {
                            reportError(rhsRegisterToken, "Operand size mismatch on 'mov' instruction.");
                        }

                        switch(lhs.second)
                        {
                            case codegen::OperandSize::Byte:
                                mOutput.write(codegen::MOV_REG_REG8, mSection);
                                break;
                            case codegen::OperandSize::Word:
                                mOutput.write(SIZE_16, mSection);
                                mOutput.write(codegen::MOV_REG_REG, mSection);
                                break;
                            case codegen::OperandSize::Long:
                                mOutput.write(codegen::MOV_REG_REG, mSection);
                                break;
                            case codegen::OperandSize::Quad:
                                mOutput.write(codegen::REX::W, mSection);
                                mOutput.write(codegen::MOV_REG_REG, mSection);
                                break;
                        }
                        mOutput.write((unsigned char)(0xC0 + lhs.first + rhs.first * 8), mSection);
                    }
                }
            },
            {
                "int", [&](){
                    unsigned char vector = parseExpression();

                    mOutput.write(codegen::INT, mSection);
                    mOutput.write(vector, mSection);
                }
            },
            {
                "syscall", [&](){
                    mOutput.write(codegen::SYSCALL, mSection);
                }
            },
            {
                "nop", [&](){
                    mOutput.write(codegen::NOP, mSection);
                }
            },
            {
                "times", [&](){
                    long long iterations = parseExpression();

                    size_t position = mPosition;
                    while (--iterations)
                    {
                        parseStatement();
                        mPosition = position;
                    }
                }
            },
        };
    }
    
    lexing::Token& Parser::current()
    {
        return mTokens[mPosition];
    }

    lexing::Token& Parser::consume()
    {
        return mTokens[mPosition++];
    }

    const lexing::Token& Parser::peek(size_t offset) const
    {
        return mTokens[mPosition + offset];
    }

    void Parser::expectToken(lexing::TokenType tokenType, std::string_view context)
    {
        auto token = current();
        if (token.getTokenType() != tokenType)
        {
            reportError(token, context);
        }
    }
    
    void Parser::reportError(const lexing::Token& token, std::string_view error) {
        std::cerr << filename << ':' << token.getSourceLocation().line << ':' << token.getSourceLocation().column << ": " << error << '\n';
        std::exit(1);
    }

    int Parser::getBinaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch (tokenType)
        {
            case lexing::TokenType::Plus:
            case lexing::TokenType::Minus:
                return 35;
            case lexing::TokenType::Star:
            case lexing::TokenType::Slash:
                return 45;
            default:
                return 0;
        }
    }

    bool Parser::isImmediate(lexing::TokenType tokenType)
    {
        return tokenType == lexing::TokenType::Immediate || tokenType == lexing::TokenType::Identifier || tokenType == lexing::TokenType::Dollar || tokenType == lexing::TokenType::DollarDollar;
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
        auto token = current();
        switch (token.getTokenType())
        {
            case lexing::TokenType::Error:
                reportError(token, "Found unknown symbol.");
                break;
                
            case lexing::TokenType::Identifier:
                parseLabel();
                break;

            case lexing::TokenType::Instruction:
            {
                InstructionParser parser = mInstructionParsers.at(consume().getText());
                parser();
                break;
            }

            default:
                reportError(token, "Expected statement, found '" + token.getText() + "'.");
        }
    }

    void Parser::parseLabel()
    {
        const std::string& name = consume().getText();

        expectToken(lexing::TokenType::Colon, "Expected ':' after a label.");
        consume();

        mOutput.addSymbol(name, mOutput.getPosition(mSection), mSection, codegen::Global(true)); // TODO: Check if global
    }

    long long Parser::parseExpression(int precedence)
    {
        long long lhs = parseImmediate();
        int binaryOperatorPrecedence;
        while (mPosition < mTokens.size() && (binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType()), binaryOperatorPrecedence > precedence))
        {
            lexing::TokenType operatorToken = consume().getTokenType();

            long long rhs = parseExpression(binaryOperatorPrecedence);

            switch(operatorToken)
            {
                case lexing::TokenType::Plus:
                    lhs += rhs;
                    break;
                case lexing::TokenType::Minus:
                    lhs -= rhs;
                    break;
                case lexing::TokenType::Star:
                    lhs *= rhs;
                    break;
                case lexing::TokenType::Slash:
                    lhs /= rhs;
                    break;
                default:
                    break;
            }
        }
        return lhs;
    }

    long long Parser::parseImmediate()
    {
        if(current().getTokenType() == lexing::TokenType::LParen)
        {
            consume();

            long long ret = parseExpression();
            
            // TODO: Use the location of LParen
            expectToken(lexing::TokenType::RParen, "Expected matching ')'.");
            consume();

            return ret;
        }
        else if (current().getTokenType() == lexing::TokenType::Immediate)
        {
            return std::stoll(consume().getText(), nullptr, 0);
        }
        else if (current().getTokenType() == lexing::TokenType::Dollar)
        {
            consume();
            return static_cast<long long>(mOutput.getPosition(mSection));
        }
        else if (current().getTokenType() == lexing::TokenType::DollarDollar)
        {
            consume();
            return static_cast<long long>(mOutput.getSectionStart(mSection));
        }
        else if (current().getTokenType() == lexing::TokenType::Identifier)
        {
            if (!mOutput.hasSymbol(current().getText())) {
                reportError(current(), "Found unknown symbol '" + current().getText() + "'.");
            }
            return static_cast<long long>(mOutput.getSymbol(consume().getText()));
        }
        else
            expectToken(lexing::TokenType::Immediate, "Expected an immediate.");

        return -1;
    }

    std::pair<long long, codegen::OperandSize> Parser::parseRegister()
    {
        constexpr int REGISTERS_PER_ENCODING = 4;

        long long index;
        for (index = 0; index < static_cast<long long>(codegen::Registers.size()); index++)
        {
            if (codegen::Registers[index] == current().getText())
            {
                break;
            }
        }
        consume();

        return std::make_pair(index / REGISTERS_PER_ENCODING, static_cast<codegen::OperandSize>(index % REGISTERS_PER_ENCODING));
    }
}