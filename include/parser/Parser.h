#ifndef VASM_PARSER_PARSER_H
#define VASM_PARSER_PARSER_H 1

#include <functional>
#include <string>

namespace Lexing
{
    class Token;
    enum class TokenType;
}

namespace Codegen
{
    class OutputFormat;
    enum class Section;
    enum class OperandSize;
}

namespace Parsing
{
    class Parser
    {
    public:
        Parser(std::string_view filename, std::vector<Lexing::Token>& tokens, Codegen::OutputFormat& output);

        void parse();

    private:
        std::string_view filename;
        std::vector<Lexing::Token>& mTokens;
        Codegen::OutputFormat& mOutput;
        size_t mPosition {0};
        Codegen::Section mSection;

        using InstructionParser = std::function<void ()>;
        std::unordered_map<std::string_view, InstructionParser> mInstructionParsers;


        Lexing::Token& current();
        Lexing::Token& consume();
        [[nodiscard]] const Lexing::Token& peek(size_t offset) const;
        
        void expectToken(Lexing::TokenType tokenType, std::string_view context);
        void reportError(const Lexing::Token& token, std::string_view error);
        
        static int getBinaryOperatorPrecedence(Lexing::TokenType tokenType);
        static bool isImmediate(Lexing::TokenType tokenType);
        
        void parseStatement();

        void parseLabel();
        long long parseExpression(int precedence = 1);
        long long parseImmediate();
        std::pair<long long, Codegen::OperandSize> parseRegister();
    };
}

#endif