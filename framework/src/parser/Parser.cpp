// Copyright 2023 solar-mist


#include "parser/Parser.h"
#include "lexer/Token.h"

#include "error/IErrorReporter.h"

#include "instruction/Builder.h"

#include "instruction/singleOperandInstruction/CallInstruction.h"
#include "instruction/singleOperandInstruction/JmpInstruction.h"
#include "instruction/singleOperandInstruction/JccInstruction.h"
#include "instruction/singleOperandInstruction/PushInstruction.h"
#include "instruction/singleOperandInstruction/PopInstruction.h"
#include "instruction/singleOperandInstruction/IncDecInstruction.h"
#include "instruction/singleOperandInstruction/DeclInstruction.h"

#include "instruction/twoOperandInstruction/MovInstruction.h"
#include "instruction/twoOperandInstruction/XchgInstruction.h"
#include "instruction/twoOperandInstruction/LogicalInstruction.h"
#include "instruction/twoOperandInstruction/OutInInstruction.h"

namespace parsing
{
    using namespace instruction;

    Parser::Parser(std::string_view filename, std::vector<lexing::Token>& tokens, error::IErrorReporter& errorReporter)
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
            { "ja",       [this]() -> InstructionPtr { return Builder<JaInstruction>()     .parse(mTokenStream); } },
            { "jae",      [this]() -> InstructionPtr { return Builder<JaeInstruction>()    .parse(mTokenStream); } },
            { "jb",       [this]() -> InstructionPtr { return Builder<JbInstruction>()     .parse(mTokenStream); } },
            { "jbe",      [this]() -> InstructionPtr { return Builder<JbeInstruction>()    .parse(mTokenStream); } },
            { "jcxz",     [this]() -> InstructionPtr { return Builder<JcxzInstruction>()   .parse(mTokenStream); } },
            { "jecxz",    [this]() -> InstructionPtr { return Builder<JecxzInstruction>()  .parse(mTokenStream); } },
            { "jrcxz",    [this]() -> InstructionPtr { return Builder<JrcxzInstruction>()  .parse(mTokenStream); } },
            { "je",       [this]() -> InstructionPtr { return Builder<JeInstruction>()     .parse(mTokenStream); } },
            { "jg",       [this]() -> InstructionPtr { return Builder<JgInstruction>()     .parse(mTokenStream); } },
            { "jge",      [this]() -> InstructionPtr { return Builder<JgeInstruction>()    .parse(mTokenStream); } },
            { "jl",       [this]() -> InstructionPtr { return Builder<JlInstruction>()     .parse(mTokenStream); } },
            { "jle",      [this]() -> InstructionPtr { return Builder<JleInstruction>()    .parse(mTokenStream); } },
            { "jna",      [this]() -> InstructionPtr { return Builder<JnaInstruction>()    .parse(mTokenStream); } },
            { "jnae",     [this]() -> InstructionPtr { return Builder<JnaeInstruction>()   .parse(mTokenStream); } },
            { "jnb",      [this]() -> InstructionPtr { return Builder<JnbInstruction>()    .parse(mTokenStream); } },
            { "jnbe",     [this]() -> InstructionPtr { return Builder<JnbeInstruction>()   .parse(mTokenStream); } },
            { "jnc",      [this]() -> InstructionPtr { return Builder<JncInstruction>()    .parse(mTokenStream); } },
            { "jne",      [this]() -> InstructionPtr { return Builder<JneInstruction>()    .parse(mTokenStream); } },
            { "jng",      [this]() -> InstructionPtr { return Builder<JngInstruction>()    .parse(mTokenStream); } },
            { "jnge",     [this]() -> InstructionPtr { return Builder<JngeInstruction>()   .parse(mTokenStream); } },
            { "jnl",      [this]() -> InstructionPtr { return Builder<JnlInstruction>()    .parse(mTokenStream); } },
            { "jnle",     [this]() -> InstructionPtr { return Builder<JnleInstruction>()   .parse(mTokenStream); } },
            { "jno",      [this]() -> InstructionPtr { return Builder<JnoInstruction>()    .parse(mTokenStream); } },
            { "jnp",      [this]() -> InstructionPtr { return Builder<JnpInstruction>()    .parse(mTokenStream); } },
            { "jns",      [this]() -> InstructionPtr { return Builder<JnsInstruction>()    .parse(mTokenStream); } },
            { "jnz",      [this]() -> InstructionPtr { return Builder<JnzInstruction>()    .parse(mTokenStream); } },
            { "jo",       [this]() -> InstructionPtr { return Builder<JoInstruction>()     .parse(mTokenStream); } },
            { "jp",       [this]() -> InstructionPtr { return Builder<JpInstruction>()     .parse(mTokenStream); } },
            { "jpe",      [this]() -> InstructionPtr { return Builder<JpeInstruction>()    .parse(mTokenStream); } },
            { "jpo",      [this]() -> InstructionPtr { return Builder<JpoInstruction>()    .parse(mTokenStream); } },
            { "js",       [this]() -> InstructionPtr { return Builder<JsInstruction>()     .parse(mTokenStream); } },
            { "jz",       [this]() -> InstructionPtr { return Builder<JzInstruction>()     .parse(mTokenStream); } },

            { "mov",      [this]() -> InstructionPtr { return Builder<MovInstruction>()    .parse(mTokenStream); } },

            { "xchg",     [this]() -> InstructionPtr { return Builder<XchgInstruction>()   .parse(mTokenStream); } },

            { "add",      [this]() -> InstructionPtr { return Builder<AddInstruction>()    .parse(mTokenStream); } },
            { "adc",      [this]() -> InstructionPtr { return Builder<AdcInstruction>()    .parse(mTokenStream); } },
            { "sbb",      [this]() -> InstructionPtr { return Builder<SbbInstruction>()    .parse(mTokenStream); } },
            { "or",       [this]() -> InstructionPtr { return Builder<OrInstruction>()     .parse(mTokenStream); } },
            { "and",      [this]() -> InstructionPtr { return Builder<AndInstruction>()    .parse(mTokenStream); } },
            { "sub",      [this]() -> InstructionPtr { return Builder<SubInstruction>()    .parse(mTokenStream); } },
            { "xor",      [this]() -> InstructionPtr { return Builder<XorInstruction>()    .parse(mTokenStream); } },
            { "cmp",      [this]() -> InstructionPtr { return Builder<CmpInstruction>()    .parse(mTokenStream); } },

            { "push",     [this]() -> InstructionPtr { return Builder<PushInstruction>()   .parse(mTokenStream); } },
            { "pop",      [this]() -> InstructionPtr { return Builder<PopInstruction>()    .parse(mTokenStream); } },

            { "inc",      [this]() -> InstructionPtr { return Builder<IncInstruction>()    .parse(mTokenStream); } },
            { "dec",      [this]() -> InstructionPtr { return Builder<DecInstruction>()    .parse(mTokenStream); } },

            { "in",       [this]() -> InstructionPtr { return Builder<InInstruction>()     .parse(mTokenStream); } },
            { "out",      [this]() -> InstructionPtr { return Builder<OutInstruction>()    .parse(mTokenStream); } },

            { "syscall",  [this]() -> InstructionPtr { return Builder<SyscallInstruction>().parse(mTokenStream); } },

            { "nop",      [this]() -> InstructionPtr { return Builder<NopInstruction>()    .parse(mTokenStream); } },


            { "db",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Byte>>().parse( mTokenStream); } },
            { "dw",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Word>>().parse( mTokenStream); } },
            { "dd",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Long>>().parse( mTokenStream); } },
            { "dq",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Quad>>().parse( mTokenStream); } },
        };
    }

    std::vector<ValuePtr> Parser::parse()
    {
        std::vector<ValuePtr> ret;

        while (mPosition < mTokens.size())
        {
            ret.push_back(parseStatement());
        }

        return ret;
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
            mErrorReporter.reportError({filename, context, token});
        }
    }
    
    ValuePtr Parser::parseStatement()
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