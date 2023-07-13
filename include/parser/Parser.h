#ifndef VASM_PARSER_PARSER_H
#define VASM_PARSER_PARSER_H 1

#include <vector>

namespace Lexing
{
    class Token;
    enum class TokenType;
}

namespace Codegen
{
    class OutputFormat;
    enum class Section;
}

namespace Parsing
{
    class Parser
    {
    public:
        Parser(std::vector<Lexing::Token>& tokens, Codegen::OutputFormat& output);

        void parse();

    private:
        std::vector<Lexing::Token>& mTokens;
        Codegen::OutputFormat& mOutput;
        int mPosition{ 0 };
        Codegen::Section mSection;


        Lexing::Token current();
        Lexing::Token consume();
        Lexing::Token peek(int offset);

        void expectToken(Lexing::TokenType tokenType);
        
        void parseStatement();

        void parseDBInst();
        void parseDWInst();
        void parseDDInst();
        void parseDQInst();
    };
}

#endif