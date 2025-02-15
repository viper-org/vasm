// Copyright 2023 solar-mist


#include "vasm/parser/Parser.h"

#include "vasm/lexer/Token.h"

#include "vasm/error/IErrorReporter.h"

#include "vasm/instruction/Builder.h"

#include "vasm/instruction/Directive.h"

#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"
#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"
#include "vasm/instruction/singleOperandInstruction/JccInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PopInstruction.h"
#include "vasm/instruction/singleOperandInstruction/IncDecInstruction.h"
#include "vasm/instruction/singleOperandInstruction/IntInstruction.h"
#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"
#include "vasm/instruction/singleOperandInstruction/Grp4Instruction.h"
#include "vasm/instruction/singleOperandInstruction/SetccInstruction.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"
#include "vasm/instruction/twoOperandInstruction/ShiftRotateInstruction.h"
#include "vasm/instruction/twoOperandInstruction/XchgInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/twoOperandInstruction/OutInInstruction.h"
#include "vasm/instruction/twoOperandInstruction/TestInstruction.h"

#include "vasm/instruction/variableOperandInstruction/IMulInstruction.h"

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

            { "leave",    [this]() -> InstructionPtr { return Builder<LeaveInstruction>()  .parse(mTokenStream); } },

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

            { "seta",     [this]() -> InstructionPtr { return Builder<SetaInstruction>()   .parse(mTokenStream); } },
            { "setae",    [this]() -> InstructionPtr { return Builder<SetaeInstruction>()  .parse(mTokenStream); } },
            { "setb",     [this]() -> InstructionPtr { return Builder<SetbInstruction>()   .parse(mTokenStream); } },
            { "setbe",    [this]() -> InstructionPtr { return Builder<SetbeInstruction>()  .parse(mTokenStream); } },
            { "sete",     [this]() -> InstructionPtr { return Builder<SeteInstruction>()   .parse(mTokenStream); } },
            { "setg",     [this]() -> InstructionPtr { return Builder<SetgInstruction>()   .parse(mTokenStream); } },
            { "setge",    [this]() -> InstructionPtr { return Builder<SetgeInstruction>()  .parse(mTokenStream); } },
            { "setl",     [this]() -> InstructionPtr { return Builder<SetlInstruction>()   .parse(mTokenStream); } },
            { "setle",    [this]() -> InstructionPtr { return Builder<SetleInstruction>()  .parse(mTokenStream); } },
            { "setna",    [this]() -> InstructionPtr { return Builder<SetnaInstruction>()  .parse(mTokenStream); } },
            { "setnae",   [this]() -> InstructionPtr { return Builder<SetnaeInstruction>() .parse(mTokenStream); } },
            { "setnb",    [this]() -> InstructionPtr { return Builder<SetnbInstruction>()  .parse(mTokenStream); } },
            { "setnbe",   [this]() -> InstructionPtr { return Builder<SetnbeInstruction>() .parse(mTokenStream); } },
            { "setnc",    [this]() -> InstructionPtr { return Builder<SetncInstruction>()  .parse(mTokenStream); } },
            { "setne",    [this]() -> InstructionPtr { return Builder<SetneInstruction>()  .parse(mTokenStream); } },
            { "setng",    [this]() -> InstructionPtr { return Builder<SetngInstruction>()  .parse(mTokenStream); } },
            { "setnge",   [this]() -> InstructionPtr { return Builder<SetngeInstruction>() .parse(mTokenStream); } },
            { "setnl",    [this]() -> InstructionPtr { return Builder<SetnlInstruction>()  .parse(mTokenStream); } },
            { "setnle",   [this]() -> InstructionPtr { return Builder<SetnleInstruction>() .parse(mTokenStream); } },
            { "setno",    [this]() -> InstructionPtr { return Builder<SetnoInstruction>()  .parse(mTokenStream); } },
            { "setnp",    [this]() -> InstructionPtr { return Builder<SetnpInstruction>()  .parse(mTokenStream); } },
            { "setns",    [this]() -> InstructionPtr { return Builder<SetnsInstruction>()  .parse(mTokenStream); } },
            { "setnz",    [this]() -> InstructionPtr { return Builder<SetnzInstruction>()  .parse(mTokenStream); } },
            { "seto",     [this]() -> InstructionPtr { return Builder<SetoInstruction>()   .parse(mTokenStream); } },
            { "setp",     [this]() -> InstructionPtr { return Builder<SetpInstruction>()   .parse(mTokenStream); } },
            { "setpe",    [this]() -> InstructionPtr { return Builder<SetpeInstruction>()  .parse(mTokenStream); } },
            { "setpo",    [this]() -> InstructionPtr { return Builder<SetpoInstruction>()  .parse(mTokenStream); } },
            { "sets",     [this]() -> InstructionPtr { return Builder<SetsInstruction>()   .parse(mTokenStream); } },
            { "setz",     [this]() -> InstructionPtr { return Builder<SetzInstruction>()   .parse(mTokenStream); } },

            { "mov",      [this]() -> InstructionPtr { return Builder<MovInstruction>()    .parse(mTokenStream); } },
            { "movzx",    [this]() -> InstructionPtr { return Builder<MovZXInstruction>()  .parse(mTokenStream); } },
            { "movsx",    [this]() -> InstructionPtr { return Builder<MovSXInstruction>()  .parse(mTokenStream); } },
            { "lea",      [this]() -> InstructionPtr { return Builder<LeaInstruction>()    .parse(mTokenStream); } },

            { "rol",      [this]() -> InstructionPtr { return Builder<RolInstruction>()    .parse(mTokenStream); } },
            { "ror",      [this]() -> InstructionPtr { return Builder<RorInstruction>()    .parse(mTokenStream); } },
            { "rcl",      [this]() -> InstructionPtr { return Builder<RclInstruction>()    .parse(mTokenStream); } },
            { "rcl",      [this]() -> InstructionPtr { return Builder<RcrInstruction>()    .parse(mTokenStream); } },
            { "shl",      [this]() -> InstructionPtr { return Builder<ShlInstruction>()    .parse(mTokenStream); } },
            { "shr",      [this]() -> InstructionPtr { return Builder<ShrInstruction>()    .parse(mTokenStream); } },
            { "sal",      [this]() -> InstructionPtr { return Builder<SalInstruction>()    .parse(mTokenStream); } },
            { "sar",      [this]() -> InstructionPtr { return Builder<SarInstruction>()    .parse(mTokenStream); } },

            { "xchg",     [this]() -> InstructionPtr { return Builder<XchgInstruction>()   .parse(mTokenStream); } },

            { "add",      [this]() -> InstructionPtr { return Builder<AddInstruction>()    .parse(mTokenStream); } },
            { "adc",      [this]() -> InstructionPtr { return Builder<AdcInstruction>()    .parse(mTokenStream); } },
            { "sbb",      [this]() -> InstructionPtr { return Builder<SbbInstruction>()    .parse(mTokenStream); } },
            { "or",       [this]() -> InstructionPtr { return Builder<OrInstruction>()     .parse(mTokenStream); } },
            { "and",      [this]() -> InstructionPtr { return Builder<AndInstruction>()    .parse(mTokenStream); } },
            { "sub",      [this]() -> InstructionPtr { return Builder<SubInstruction>()    .parse(mTokenStream); } },
            { "xor",      [this]() -> InstructionPtr { return Builder<XorInstruction>()    .parse(mTokenStream); } },
            { "cmp",      [this]() -> InstructionPtr { return Builder<CmpInstruction>()    .parse(mTokenStream); } },

            { "test",     [this]() -> InstructionPtr { return Builder<TestInstruction>()   .parse(mTokenStream); } },
            { "not",      [this]() -> InstructionPtr { return Builder<NotInstruction>()   .parse(mTokenStream); } },
            { "neg",      [this]() -> InstructionPtr { return Builder<NegInstruction>()   .parse(mTokenStream); } },
            { "mul",      [this]() -> InstructionPtr { return Builder<MulInstruction>()   .parse(mTokenStream); } },
            { "imul",     [this]() -> InstructionPtr { return Builder<IMulInstruction>()   .parse(mTokenStream); } },
            { "div",      [this]() -> InstructionPtr { return Builder<DivInstruction>()   .parse(mTokenStream); } },
            { "idiv",     [this]() -> InstructionPtr { return Builder<IDivInstruction>()   .parse(mTokenStream); } },

            { "push",     [this]() -> InstructionPtr { return Builder<PushInstruction>()   .parse(mTokenStream); } },
            { "pop",      [this]() -> InstructionPtr { return Builder<PopInstruction>()    .parse(mTokenStream); } },

            { "inc",      [this]() -> InstructionPtr { return Builder<IncInstruction>()    .parse(mTokenStream); } },
            { "dec",      [this]() -> InstructionPtr { return Builder<DecInstruction>()    .parse(mTokenStream); } },

            { "in",       [this]() -> InstructionPtr { return Builder<InInstruction>()     .parse(mTokenStream); } },
            { "out",      [this]() -> InstructionPtr { return Builder<OutInstruction>()    .parse(mTokenStream); } },

            { "syscall",  [this]() -> InstructionPtr { return Builder<SyscallInstruction>().parse(mTokenStream); } },
            { "int",      [this]() -> InstructionPtr { return Builder<IntInstruction>()    .parse(mTokenStream); } },

            { "nop",      [this]() -> InstructionPtr { return Builder<NopInstruction>()    .parse(mTokenStream); } },


            { "db",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Byte, "db">>().parse( mTokenStream); } },
            { "dw",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Word, "dw">>().parse( mTokenStream); } },
            { "dd",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Long, "dl">>().parse( mTokenStream); } },
            { "dq",       [this]() -> InstructionPtr { return Builder<DeclInstruction<codegen::OperandSize::Quad, "dq">>().parse( mTokenStream); } },
        };
    }

    std::vector<ValuePtr> Parser::parse()
    {
        std::vector<ValuePtr> ret;

        while (current().getTokenType() != lexing::TokenType::End)
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

            case lexing::TokenType::Extern:
            {
                consume();
                return std::make_unique<ExternDirective>(consume().getText());
            }
            case lexing::TokenType::Section:
            {
                consume();
                return std::make_unique<SectionDirective>(consume().getText());
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