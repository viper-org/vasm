// Copyright 2023 solar-mist


#include "parser/NewParser.h"

#include "lexer/Token.h"

#include "error/IErrorReporter.h"

#include "instruction/Builder.h"

#include "instruction/noOperandInstruction/RetInstruction.h"

#include "instruction/singleOperandInstruction/PushInstruction.h"
#include "instruction/singleOperandInstruction/PopInstruction.h"

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
            { "ret",  [this]() -> InstructionPtr { return Builder<RetInstruction>().parse( mTokenStream); } },
            { "push", [this]() -> InstructionPtr { return Builder<PushInstruction>().parse(mTokenStream); } },
            { "pop",  [this]() -> InstructionPtr { return Builder<PopInstruction>().parse( mTokenStream); } },
        };
    }

    std::vector<InstructionPtr> NewParser::parse()
    {
        std::vector<InstructionPtr> ret;

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
    
    InstructionPtr NewParser::parseStatement()
    {
        auto token = current();
        switch (token.getTokenType())
        {
            case lexing::TokenType::Error:
                mErrorReporter.reportError({filename, "Found unknown symbol.", token});
                break;

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