// Copyright 2023 solar-mist


#include "parser/NewParser.h"

#include "lexer/Token.h"

#include "error/IErrorReporter.h"

#include "instruction/Builder.h"

#include "instruction/singleOperandInstruction/CallInstruction.h"
#include "instruction/singleOperandInstruction/JmpInstruction.h"
#include "instruction/singleOperandInstruction/PushInstruction.h"
#include "instruction/singleOperandInstruction/PopInstruction.h"
#include "instruction/singleOperandInstruction/DeclInstruction.h"

#include "instruction/twoOperandInstruction/MovInstruction.h"
#include "instruction/twoOperandInstruction/AddSubInstruction.h"

namespace parsing
{
    using namespace instruction;

    NewParser::NewParser(std::string_view filename, std::vector<lexing::Token>& tokens, error::IErrorReporter& errorReporter)
        : filename {filename}
        , mTokens(tokens)
        , mErrorReporter {errorReporter}
        , mTokenStream({ &mTokens, &mPosition })
        , mPosition(0)
    {
        mInstructionParsers = {
            { "ret",      [this]() -> InstructionPtr { return Builder<RetInstruction>()    .parse(mTokenStream); } },
            { "call",     [this]() -> InstructionPtr { return Builder<CallInstruction>()   .parse(mTokenStream); } },
            { "jmp",      [this]() -> InstructionPtr { return Builder<JmpInstruction>()    .parse(mTokenStream); } },
            { "mov",      [this]() -> InstructionPtr { return Builder<MovInstruction>()    .parse(mTokenStream); } },
            { "add",      [this]() -> InstructionPtr { return Builder<AddInstruction>()    .parse(mTokenStream); } },
            { "sub",      [this]() -> InstructionPtr { return Builder<SubInstruction>()    .parse(mTokenStream); } },
            { "push",     [this]() -> InstructionPtr { return Builder<PushInstruction>()   .parse(mTokenStream); } },
            { "pop",      [this]() -> InstructionPtr { return Builder<PopInstruction>()    .parse(mTokenStream); } },
            { "syscall",  [this]() -> InstructionPtr { return Builder<SyscallInstruction>().parse( mTokenStream); } },

            { "db",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Byte>>().parse( mTokenStream); } },
            { "dw",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Word>>().parse( mTokenStream); } },
            { "dd",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Long>>().parse( mTokenStream); } },
            { "dq",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Quad>>().parse( mTokenStream); } },
        };
    }

    std::vector<ValuePtr> NewParser::parse()
    {
        std::vector<ValuePtr> ret;

        while (mPosition < mTokens.size())
        {
            ret.push_back(parseStatement());
        }

        return ret;
    }

    lexing::Token& NewParser::current()
    {
        return mTokens[mPosition];
    }

    lexing::Token& NewParser::consume()
    {
        return mTokens[mPosition++];
    }

    const lexing::Token& NewParser::peek(size_t offset) const
    {
        return mTokens[mPosition + offset];
    }

    void NewParser::expectToken(lexing::TokenType tokenType, std::string_view context)
    {
        auto token = current();
        if (token.getTokenType() != tokenType)
        {
            mErrorReporter.reportError({filename, context, token});
        }
    }
    
    ValuePtr NewParser::parseStatement()
    {
        auto token = current();
        switch (token.getTokenType())
        {
            case lexing::TokenType::Error:
                mErrorReporter.reportError({filename, "Found unknown symbol.", token});
                break;

            case lexing::TokenType::Identifier:
            {
                return Builder<Label>().parse(mTokenStream);
            }

            case lexing::TokenType::Instruction:
            {
                InstructionParser parser = mInstructionParsers.at(consume().getText());
                return parser();
            }

            default:
                mErrorReporter.reportError({filename, "Expected statement, found '" + token.getText() + "'.", token});
        }
        return nullptr;
    }
}