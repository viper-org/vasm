#include "Test.h"

#include "parser/Parser.h"

#include "lexer/Token.h"

#include "codegen/FakeOutputFormat.h"

namespace ParserTests
{
    using namespace codegen::fakes;

    using Token = std::pair<lexing::TokenType, std::string>;

    struct TestCase
    {
        std::vector<Token> input;
        std::vector<unsigned char> expectedCode;
        std::vector<unsigned char> expectedData;
        std::vector<FakeSymbol> expectedSymbols;
        std::vector<FakeRelocation> expectedRelocations;

        TestCase& setInput(              const std::vector<Token>& newInput)                { input               = newInput;       return *this; }
        TestCase& setExpectedCode(       const std::vector<unsigned char>& newCode)         { expectedCode        = newCode;        return *this; }
        TestCase& setExpectedData(       const std::vector<unsigned char>& newData)         { expectedData        = newData;        return *this; }
        TestCase& setExpectedSymbols(    const std::vector<FakeSymbol>& newSymbols)         { expectedSymbols     = newSymbols;     return *this; }
        TestCase& setExpectedRelocations(const std::vector<FakeRelocation>& newRelocations) { expectedRelocations = newRelocations; return *this; }
    };
    using TestCases = std::vector<TestCase>;

    void check(const TestCases& testCases)
    {
        for (const auto& testCase : testCases)
        {
            std::vector<lexing::Token> tokens;
            std::transform(testCase.input.begin(), testCase.input.end(), std::back_inserter(tokens), [](const Token& token) {
                return lexing::Token({0, 0}, token.first, token.second);
            });

            FakeOutputFormat output;

            parsing::Parser parser("", tokens, output);
            parser.parse();

            REQUIRE(testCase.expectedCode        == output.getCode());
            REQUIRE(testCase.expectedData        == output.getData());

            REQUIRE(testCase.expectedSymbols.size()     == output.getSymbols().size());
            REQUIRE(testCase.expectedRelocations.size() == output.getRelocations().size());

            for (int i = 0; i < testCase.expectedSymbols.size(); i++)
            {
                REQUIRE(testCase.expectedSymbols.at(i) == output.getSymbols().at(i));
            }

            for (int i = 0; i < testCase.expectedRelocations.size(); i++)
            {
                REQUIRE(testCase.expectedRelocations.at(i) == output.getRelocations().at(i));
            }
        }
    }

    TEST(Symbols, ParserTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput({
                    { lexing::TokenType::Identifier, "hi" },
                    { lexing::TokenType::Colon, "" },
                })
                .setExpectedCode({})
                .setExpectedData({})
                .setExpectedSymbols({ FakeSymbol{ "hi" } })
                .setExpectedRelocations({})

        };

        check(testCases);
    }
}