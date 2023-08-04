#ifndef VASM_PARSER_PARSER_H
#define VASM_PARSER_PARSER_H 1

#include <functional>
#include <string>

namespace lexing
{
    class Token;
    enum class TokenType;
}

namespace codegen
{
    class IOutputFormat;
    enum class Section;
    enum class OperandSize;
}

namespace parsing
{
    class Parser
    {
    public:
        Parser(std::string_view filename, std::vector<lexing::Token>& tokens, codegen::IOutputFormat& output);

        void parse();

    private:
        std::string_view filename;
        std::vector<lexing::Token>& mTokens;
        codegen::IOutputFormat& mOutput;
        size_t mPosition {0};
        codegen::Section mSection;

        using InstructionParser = std::function<void ()>;
        std::unordered_map<std::string_view, InstructionParser> mInstructionParsers;


        lexing::Token& current();
        lexing::Token& consume();
        [[nodiscard]] const lexing::Token& peek(size_t offset) const;
        
        void expectToken(lexing::TokenType tokenType, std::string_view context);
        void reportError(const lexing::Token& token, std::string_view error);
        
        static int getBinaryOperatorPrecedence(lexing::TokenType tokenType);
        static bool isImmediate(lexing::TokenType tokenType);
        
        void parseStatement();

        void parseLabel();
        long long parseExpression(int precedence = 1);
        long long parseImmediate();
        std::pair<long long, codegen::OperandSize> parseRegister();
    };
}

#endif