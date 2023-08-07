#include "Test.h"

#include "error/ErrorReporter.h"
#include "parser/Parser.h"

#include "lexer/Token.h"

#include "codegen/FakeOutputFormat.h"
#include "codegen/Opcodes.h"

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
            error::ErrorReporter errorReporter; // TODO: Use FakeErrorReporter

            parsing::Parser parser("test", tokens, output, errorReporter);
            parser.parse();

            REQUIRE(testCase.expectedCode.size() == output.getCode().size());
            REQUIRE(testCase.expectedData.size() == output.getData().size());

            for (int i = 0; i < testCase.expectedCode.size(); i++)
            {
                REQUIRE(testCase.expectedCode.at(i) == output.getCode().at(i));
            }

            for (int i = 0; i < testCase.expectedData.size(); i++)
            {
                REQUIRE(testCase.expectedData.at(i) == output.getData().at(i));
            }

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

    namespace instructions
    {
        TEST(DeclInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "db" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "dw" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ 0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "dd" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ 0x00, 0x00, 0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "dq" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(JumpInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "jmp" },
                        { lexing::TokenType::Dollar, "" },
                    })
                    .setExpectedCode({ codegen::JMP_REL8, static_cast<unsigned char>(-2) }) // Instruction is 2 bytes
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(CallInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "call" },
                        { lexing::TokenType::Dollar, "" },
                    })
                    .setExpectedCode({ codegen::CALL_REL32, static_cast<unsigned char>(-5), 0xFF, 0xFF, 0xFF }) // Instruction is 5 bytes
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(RetInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "ret" }
                    })
                    .setExpectedCode({ codegen::RET })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(MovInst, ParserTests)
        {
            TestCases testCases = {
                
                /*****************************************************************************/
                /*                                 Reg,Reg                                   */
                /*****************************************************************************/
                
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "al" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "al" },
                    })
                    .setExpectedCode({ codegen::MOV_REG_REG8, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "ax" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::MOV_REG_REG, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "eax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "eax" },
                    })
                    .setExpectedCode({ codegen::MOV_REG_REG, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "rax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "rax" },
                    })
                    .setExpectedCode({ codegen::REX::W, codegen::MOV_REG_REG, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                
                /*****************************************************************************/
                /*                                 Reg,Imm                                   */
                /*****************************************************************************/
                
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "al" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ codegen::MOV_REG_IMM8, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::MOV_REG_IMM, 0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "eax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ codegen::MOV_REG_IMM, 0x00, 0x00,0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "mov" },
                        { lexing::TokenType::Register, "rax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0" },
                    })
                    .setExpectedCode({ codegen::REX::W, codegen::MOV_REG_IMM, 0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(LeaInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::Colon, "" },
                        { lexing::TokenType::Instruction, "lea" },
                        { lexing::TokenType::Instruction, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::LBracket, "" },
                        { lexing::TokenType::Identifier, "rel" },
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::RBracket, "" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::LEA, 0x05, static_cast<unsigned char>(-7), 0xFF, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({ FakeSymbol{"a"} })
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::Colon, "" },
                        { lexing::TokenType::Instruction, "lea" },
                        { lexing::TokenType::Instruction, "eax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::LBracket, "" },
                        { lexing::TokenType::Identifier, "rel" },
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::RBracket, "" },
                    })
                    .setExpectedCode({ codegen::LEA, 0x05, static_cast<unsigned char>(-6), 0xFF, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({ FakeSymbol{"a"} })
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::Colon, "" },
                        { lexing::TokenType::Instruction, "lea" },
                        { lexing::TokenType::Instruction, "rax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::LBracket, "" },
                        { lexing::TokenType::Identifier, "rel" },
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::RBracket, "" },
                    })
                    .setExpectedCode({ codegen::REX::W, codegen::LEA, 0x05, static_cast<unsigned char>(-7), 0xFF, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({ FakeSymbol{"a"} })
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::Colon, "" },
                        { lexing::TokenType::Instruction, "lea" },
                        { lexing::TokenType::Instruction, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::LBracket, "" },
                        { lexing::TokenType::Identifier, "rel" },
                        { lexing::TokenType::Identifier, "a" },
                        { lexing::TokenType::RBracket, "" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::LEA, 0x05, static_cast<unsigned char>(-7), 0xFF, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({ FakeSymbol{"a"} })
                    .setExpectedRelocations({})

            };

            check(testCases);
        }

        TEST(AddInst, ParserTests)
        {
            TestCases testCases = {

                /*****************************************************************************/
                /*                                 Reg,Reg                                   */
                /*****************************************************************************/

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "al" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "al" },
                    })
                    .setExpectedCode({ codegen::ADD_REG_REG8, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "ax" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::ADD_REG_REG, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "eax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "eax" },
                    })
                    .setExpectedCode({ codegen::ADD_REG_REG, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "rax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Register, "rax" },
                    })
                    .setExpectedCode({ codegen::REX::W, codegen::ADD_REG_REG, 0xC0 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),


                /*****************************************************************************/
                /*                                 Reg,Imm8                                  */
                /*****************************************************************************/

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "al" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "1" },
                    })
                    .setExpectedCode({ codegen::ADD_REG8_IMM8, 0xC0, 0x01 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "1" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::ADD_REG_IMM8, 0xC0, 0x01 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "eax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "1" },
                    })
                    .setExpectedCode({ codegen::ADD_REG_IMM8, 0xC0, 0x01 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "rax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "1" },
                    })
                    .setExpectedCode({ codegen::REX::W, codegen::ADD_REG_IMM8, 0xC0, 0x01 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),


                /*****************************************************************************/
                /*                                 Reg,Imm                                   */
                /*****************************************************************************/

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "ax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0xFFFF" },
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::ADD_REG_IMM, 0xC0, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "eax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0xFFFFFFFF" },
                    })
                    .setExpectedCode({ codegen::ADD_REG_IMM, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),
                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "add" },
                        { lexing::TokenType::Register, "rax" },
                        { lexing::TokenType::Comma, "" },
                        { lexing::TokenType::Immediate, "0xFFFFFFFF" },
                    })
                    .setExpectedCode({ codegen::REX::W, codegen::ADD_REG_IMM, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(PushInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "push" },
                        { lexing::TokenType::Register,    "ax" }
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::PUSH_REG })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "push" },
                        { lexing::TokenType::Register,    "rax" }
                    })
                    .setExpectedCode({ codegen::PUSH_REG })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),


                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "push" },
                        { lexing::TokenType::Immediate,    "0" }
                    })
                    .setExpectedCode({ codegen::PUSH_IMM8, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "push" },
                        { lexing::TokenType::Immediate,    "0xFFFF" }
                    })
                    .setExpectedCode({ codegen::PUSH_IMM, 0xFF, 0xFF })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(PopInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "pop" },
                        { lexing::TokenType::Register,    "ax" }
                    })
                    .setExpectedCode({ codegen::SIZE_PREFIX, codegen::POP_REG })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "pop" },
                        { lexing::TokenType::Register,    "rax" }
                    })
                    .setExpectedCode({ codegen::POP_REG })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({}),

            };

            check(testCases);
        }

        TEST(IntInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "int" },
                        { lexing::TokenType::Immediate,   "0" }
                    })
                    .setExpectedCode({ codegen::INT, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({})

            };

            check(testCases);
        }

        TEST(SyscallInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "syscall" }
                    })
                    .setExpectedCode({ codegen::SYSCALL & 0xFF, codegen::SYSCALL >> 8 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({})

            };

            check(testCases);
        }

        TEST(NopInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "nop" }
                    })
                    .setExpectedCode({ codegen::NOP })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({})

            };

            check(testCases);
        }

        TEST(TimesInst, ParserTests)
        {
            TestCases testCases = {

                TestCase()
                    .setInput({
                        { lexing::TokenType::Instruction, "times" },
                        { lexing::TokenType::Immediate,   "6" },
                        { lexing::TokenType::Instruction, "db" },
                        { lexing::TokenType::Immediate,   "0" },
                    })
                    .setExpectedCode({ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 })
                    .setExpectedData({})
                    .setExpectedSymbols({})
                    .setExpectedRelocations({})

            };

            check(testCases);
        }
    }
}