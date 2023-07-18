#include <parser/Parser.h>

#include <lexer/Token.h>

#include <codegen/OutputFormat.h>
#include <codegen/Opcodes.h>

#include <iostream>

namespace Parsing
{
    constexpr unsigned char SIZE_16 = 0x66;

    Parser::Parser(std::string_view filename, std::vector<Lexing::Token>& tokens, Codegen::OutputFormat& output)
        : filename {filename}, mTokens {tokens}, mOutput {output}, mPosition {0}, mSection {Codegen::Section::Text}
    {
        mInstructionParsers = {
            {
                "db", [&](){
                    if (current().getTokenType() == Lexing::TokenType::String)
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

                    mOutput.write(Codegen::JMP_REL8, mSection);
                    mOutput.write(value, mSection);
                }
            },
            {
                "call", [&](){
                    unsigned int value = parseExpression() - mOutput.getPosition(mSection) - 5; // Subtract size of the instruction itself

                    mOutput.write(Codegen::CALL_REL32, mSection);
                    mOutput.write(value, mSection);
                }
            },
            {
                "ret", [&](){
                    mOutput.write(Codegen::RET, mSection);
                }
            },
            {
                "lea", [&](){
                    auto registerToken = current();
                    std::pair<long long, Codegen::OperandSize> lhs = parseRegister();
                    
                    expectToken(Lexing::TokenType::Comma, "Expected ',' after lea destination register.");
                    consume();
                    
                    expectToken(Lexing::TokenType::LBracket, "Expected '[' before lea source expression.");
                    auto bracketStartToken = consume();
                    
                    // TODO: Maybe a proper keyword in lexer?
                    bool rel = false;
                    if (current().getText() == "rel") {
                        consume();
                        rel = true;
                    }
                    
                    expectToken(Lexing::TokenType::Identifier, "Expected an identifier in lea source expression.");
                    long long labelAddr = parseImmediate();
                    
                    expectToken(Lexing::TokenType::RBracket, "Expected ']' after lea source expression.");
                    consume();
                    
                    if (!rel) {
                        reportError(bracketStartToken, "Not relative lea instruction is not supported.");
                    }
                    
                    int extra_bytes = 0;
                    auto start = mOutput.getPosition(mSection);
                    switch (lhs.second) {
                        case Codegen::OperandSize::Byte:
                            reportError(registerToken, "Unsupported operand size for 'lea' instruction.");
                            break;
                        case Codegen::OperandSize::Quad:
                            mOutput.write(Codegen::REX::W, mSection);
                            extra_bytes = 1;
                            [[fallthrough]];
                        case Codegen::OperandSize::Word:
                        case Codegen::OperandSize::Long:
                            mOutput.write(Codegen::LEA, mSection);
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
                                unsigned int value = labelAddr - start - (6 + extra_bytes);
                                mOutput.write(value, mSection);
                            }
                            break;
                    }
                }
            },
            {
                "mov", [&](){
                    std::pair<long long, Codegen::OperandSize> lhs = parseRegister();

                    expectToken(Lexing::TokenType::Comma, "Expected ',' after mov destination register.");
                    consume();

                    if (isImmediate(current().getTokenType()))
                    {
                        Lexing::Token* reloc = nullptr;
                        if (current().getTokenType() == Lexing::TokenType::Identifier) // Emit a relocation for symbols
                        {
                            reloc = &current();
                        }

                        long long immediate = parseExpression();
                        switch (lhs.second)
                        {
                            case Codegen::OperandSize::Byte:
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM8 + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                }
                                mOutput.write((unsigned char)immediate, mSection);
                                break;
                            case Codegen::OperandSize::Word:
                                mOutput.write(SIZE_16, mSection);
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                }
                                mOutput.write((unsigned short)immediate, mSection);
                                break;
                            case Codegen::OperandSize::Long:
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                }
                                mOutput.write((unsigned int)immediate, mSection);
                                break;
                            case Codegen::OperandSize::Quad:
                                mOutput.write(Codegen::REX::W, mSection);
                                mOutput.write((unsigned char)(Codegen::MOV_REG_IMM + lhs.first), mSection);
                                if (reloc)
                                {
                                    mOutput.relocSymbol(reloc->getText(), mSection);
                                }
                                mOutput.write((unsigned long)immediate, mSection);
                                break;
                        }
                    }
                    else if (current().getTokenType() == Lexing::TokenType::Register)
                    {
                        auto rhsRegisterToken = current();
                        std::pair<long long, Codegen::OperandSize> rhs = parseRegister();
                        if (lhs.second != rhs.second)
                        {
                            reportError(rhsRegisterToken, "Operand size mismatch on 'mov' instruction.");
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
                "syscall", [&](){
                    mOutput.write(Codegen::SYSCALL, mSection);
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
    
    Lexing::Token& Parser::current()
    {
        return mTokens[mPosition];
    }

    Lexing::Token& Parser::consume()
    {
        return mTokens[mPosition++];
    }

    const Lexing::Token& Parser::peek(size_t offset) const
    {
        return mTokens[mPosition + offset];
    }

    void Parser::expectToken(Lexing::TokenType tokenType, std::string_view context)
    {
        auto token = current();
        if (token.getTokenType() != tokenType)
        {
            reportError(token, context);
        }
    }
    
    void Parser::reportError(const Lexing::Token& token, std::string_view error) {
        std::cerr << filename << ':' << token.location.line << ':' << token.location.column << ": " << error << '\n';
        std::exit(1);
    }

    int Parser::getBinaryOperatorPrecedence(Lexing::TokenType tokenType)
    {
        switch (tokenType)
        {
            case Lexing::TokenType::Plus:
            case Lexing::TokenType::Minus:
                return 35;
            case Lexing::TokenType::Star:
            case Lexing::TokenType::Slash:
                return 45;
            default:
                return 0;
        }
    }

    bool Parser::isImmediate(Lexing::TokenType tokenType)
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
        auto token = current();
        switch (token.getTokenType())
        {
            case Lexing::TokenType::Error:
                reportError(token, "Found unknown symbol.");
                break;
                
            case Lexing::TokenType::Identifier:
                parseLabel();
                break;

            case Lexing::TokenType::Instruction:
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

        expectToken(Lexing::TokenType::Colon, "Expected ':' after a label.");
        consume();

        mOutput.addSymbol(name, mOutput.getPosition(mSection), mSection, Codegen::Global(true)); // TODO: Check if global
    }

    long long Parser::parseExpression(int precedence)
    {
        long long lhs = parseImmediate();
        int binaryOperatorPrecedence;
        while (mPosition < mTokens.size() && (binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType()), binaryOperatorPrecedence > precedence))
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
                case Lexing::TokenType::Star:
                    lhs *= rhs;
                    break;
                case Lexing::TokenType::Slash:
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
        if(current().getTokenType() == Lexing::TokenType::LParen)
        {
            consume();

            long long ret = parseExpression();
            
            // TODO: Use the location of LParen
            expectToken(Lexing::TokenType::RParen, "Expected matching ')'.");
            consume();

            return ret;
        }
        else if (current().getTokenType() == Lexing::TokenType::Immediate)
        {
            return std::stoll(consume().getText(), nullptr, 0);
        }
        else if (current().getTokenType() == Lexing::TokenType::Dollar)
        {
            consume();
            return static_cast<long long>(mOutput.getPosition(mSection));
        }
        else if (current().getTokenType() == Lexing::TokenType::DollarDollar)
        {
            consume();
            return static_cast<long long>(mOutput.getSectionStart(mSection));
        }
        else if (current().getTokenType() == Lexing::TokenType::Identifier)
        {
            if (!mOutput.hasSymbol(current().getText())) {
                reportError(current(), "Found unknown symbol '" + current().getText() + "'.");
            }
            return static_cast<long long>(mOutput.getSymbol(consume().getText()));
        }
        else
            expectToken(Lexing::TokenType::Immediate, "Expected an immediate.");

        return -1;
    }

    std::pair<long long, Codegen::OperandSize> Parser::parseRegister()
    {
        constexpr int REGISTERS_PER_ENCODING = 4;

        long long index;
        for (index = 0; index < static_cast<long long>(Codegen::Registers.size()); index++)
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