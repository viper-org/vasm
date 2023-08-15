#ifndef VASM_PARSER_NEW_PARSER_H
#define VASM_PARSER_NEW_PARSER_H 1

#include "vasm/instruction/Value.h"
#include "vasm/instruction/Builder.h"

#include <functional>
#include <memory>
#include <string>

namespace lexing
{
    class Token;
    enum class TokenType;
}

namespace error
{
    class IErrorReporter;
}

namespace parsing
{
    class Parser
    {
    public:
        Parser(std::string_view filename, std::vector<lexing::Token>& tokens,  error::IErrorReporter& errorReporter);

        std::vector<instruction::ValuePtr> parse();

    private:
        std::string_view filename;
        std::vector<lexing::Token>& mTokens;
        error::IErrorReporter& mErrorReporter;
        size_t mPosition {0};
        instruction::TokenStream mTokenStream;

        using InstructionParser = std::function<instruction::ValuePtr()>;
        std::unordered_map<std::string_view, InstructionParser> mInstructionParsers;


        lexing::Token& current();
        lexing::Token& consume();
        [[nodiscard]] const lexing::Token& peek(size_t offset) const;
        
        void expectToken(lexing::TokenType tokenType, std::string_view context);
        
        instruction::ValuePtr parseStatement();
    };
}

#endif