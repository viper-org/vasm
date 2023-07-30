#include "Test.h"

#include "lexer/Lexer.h"
#include "lexer/Token.h"

namespace LexerTests
{
    struct TestCase
    {
        std::string input;
        std::vector<Lexing::TokenType> expectedTokens;
        std::vector<std::string> expectedStrings;

        TestCase& setInput(const    std::string&                          newInput)           { input           = newInput;           return *this; }
        TestCase& setExpectedTokens(const std::vector<Lexing::TokenType>& newExpectedTokens)  { expectedTokens  = newExpectedTokens;  return *this; }
        TestCase& setExpectedStrings(const std::vector<std::string>&      newExpectedStrings) { expectedStrings = newExpectedStrings; return *this; }
    };
    using TestCases = std::vector<TestCase>;

    void check(const TestCases& testCases)
    {
        for (const auto& testCase : testCases)
        {
            Lexing::Lexer lexer(testCase.input);
            const auto tokens = lexer.lex();

            REQUIRE(tokens.size() == testCase.expectedTokens.size());

            size_t i = 0;
            for (const auto& token : tokens)
            {
                REQUIRE(token.getTokenType() == testCase.expectedTokens.at(i));
                REQUIRE(token.getText()      == testCase.expectedStrings.at(i++));
            }
        }
    }

    TEST(Dollars, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("$")
                .setExpectedTokens({ Lexing::TokenType::Dollar })
                .setExpectedStrings({ "" }),
            
            TestCase()
                .setInput("$$")
                .setExpectedTokens({ Lexing::TokenType::DollarDollar })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput("$$$")
                .setExpectedTokens({ Lexing::TokenType::DollarDollar, Lexing::TokenType::Dollar })
                .setExpectedStrings({ "", "" }),

            TestCase()
                .setInput("$ $$")
                .setExpectedTokens({ Lexing::TokenType::Dollar, Lexing::TokenType::DollarDollar })
                .setExpectedStrings({ "", "" }),

            TestCase()
                .setInput("$$$$")
                .setExpectedTokens({ Lexing::TokenType::DollarDollar, Lexing::TokenType::DollarDollar })
                .setExpectedStrings({ "", "" }),

        };

        check(testCases);
    }

    TEST(Operators, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("+")
                .setExpectedTokens({ Lexing::TokenType::Plus })
                .setExpectedStrings({ "" }),
            TestCase()
                .setInput("-")
                .setExpectedTokens({ Lexing::TokenType::Minus })
                .setExpectedStrings({ "" }),
            TestCase()
                .setInput("*")
                .setExpectedTokens({ Lexing::TokenType::Star })
                .setExpectedStrings({ "" }),
            TestCase()
                .setInput("/")
                .setExpectedTokens({ Lexing::TokenType::Slash })
                .setExpectedStrings({ "" }),

        };

        check(testCases);
    }

    TEST(Brackets, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("(")
                .setExpectedTokens({ Lexing::TokenType::LParen })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput(")")
                .setExpectedTokens({ Lexing::TokenType::RParen })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput("[")
                .setExpectedTokens({ Lexing::TokenType::LBracket })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput("]")
                .setExpectedTokens({ Lexing::TokenType::RBracket })
                .setExpectedStrings({ "" }),

        };

        check(testCases);
    }

    TEST(Punctuation, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput(":")
                .setExpectedTokens({ Lexing::TokenType::Colon })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput(",")
                .setExpectedTokens({ Lexing::TokenType::Comma })
                .setExpectedStrings({ "" }),

        };

        check(testCases);
    }

    TEST(Immediates, LexerTests)
    {
        TestCases testCases = {
            
            /*****************************************************************************/
            /*                                 Decimal                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("0")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0" }),

            TestCase()
                .setInput("1")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "1" }),

            TestCase()
                .setInput("1234567890")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "1234567890" }),

            TestCase()
                .setInput("123A")
                .setExpectedTokens({ Lexing::TokenType::Immediate, Lexing::TokenType::Identifier })
                .setExpectedStrings({ "123", "A" }),

            /*****************************************************************************/
            /*                               Hexadecimal                                 */
            /*****************************************************************************/

            TestCase()
                .setInput("0x0")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0x0" }),

            TestCase()
                .setInput("0x1")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0x1" }),

            TestCase()
                .setInput("0x1234567890ABCDEFabcdef")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0x1234567890ABCDEFabcdef" }),

            TestCase()
                .setInput("0x123G")
                .setExpectedTokens({ Lexing::TokenType::Immediate, Lexing::TokenType::Identifier })
                .setExpectedStrings({ "0x123", "G" }),

            /*****************************************************************************/
            /*                                   Octal                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("00")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "00" }),

            TestCase()
                .setInput("01")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "01" }),

            TestCase()
                .setInput("012345670")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "012345670" }),

            TestCase()
                .setInput("01238")
                .setExpectedTokens({ Lexing::TokenType::Immediate, Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0123", "8" }),

            /*****************************************************************************/
            /*                                  Binary                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("0b0")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b0" }),

            TestCase()
                .setInput("0b1")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b1" }),

            TestCase()
                .setInput("0b10")
                .setExpectedTokens({ Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b10" }),

            TestCase()
                .setInput("0b1012")
                .setExpectedTokens({ Lexing::TokenType::Immediate, Lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b101", "2" }),

        };

        check(testCases);
    }

    TEST(Strings, LexerTests)
    {
        TestCases testCases = {
            
            /*****************************************************************************/
            /*                                Printable                                  */
            /*****************************************************************************/

            TestCase()
                .setInput("\"A\"")
                .setExpectedTokens({ Lexing::TokenType::String })
                .setExpectedStrings({ "A" }),

            TestCase()
                .setInput("\"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz\"")
                .setExpectedTokens({ Lexing::TokenType::String })
                .setExpectedStrings({ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz" }),

            TestCase()
                .setInput("\"!£$%^&*()-=_+[]{};'#:@~,./<>?|`¬\"")
                .setExpectedTokens({ Lexing::TokenType::String })
                .setExpectedStrings({ "!£$%^&*()-=_+[]{};'#:@~,./<>?|`¬" }),

            /*****************************************************************************/
            /*                                 Escapes                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("\"" "\\'" "\"")
                .setExpectedTokens({ Lexing::TokenType::String })
                .setExpectedStrings({ "'" }),

            TestCase()
                .setInput("\"" "\\n" "\"")
                .setExpectedTokens({ Lexing::TokenType::String })
                .setExpectedStrings({ "\n" }),

            TestCase()
                .setInput("\"" "\\\\" "\"")
                .setExpectedTokens({ Lexing::TokenType::String })
                .setExpectedStrings({ "\\" }),

        };

        check(testCases);
    }

    TEST(Identifiers, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("a")
                .setExpectedTokens({ Lexing::TokenType::Identifier })
                .setExpectedStrings({ "a" }),

            TestCase()
                .setInput("_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_")
                .setExpectedTokens({ Lexing::TokenType::Identifier })
                .setExpectedStrings({ "_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_" }),
            
            TestCase()
                .setInput("a a")
                .setExpectedTokens({ Lexing::TokenType::Identifier, Lexing::TokenType::Identifier })
                .setExpectedStrings({ "a", "a" }),

        };

        check(testCases);
    }

    TEST(Instructions, LexerTests)
    {
        TestCases testCases = {
            
            /*****************************************************************************/
            /*                              Declarations                                 */
            /*****************************************************************************/

            TestCase()
                .setInput("db")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "db" }),

            TestCase()
                .setInput("dw")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "dw" }),

            TestCase()
                .setInput("dd")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "dd" }),
            
            TestCase()
                .setInput("dq")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "dq" }),


            /*****************************************************************************/
            /*                                Branches                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("jmp")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "jmp" }),

            TestCase()
                .setInput("call")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "call" }),

            TestCase()
                .setInput("ret")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "ret" }),

            
            /*****************************************************************************/
            /*                                    Loads                                  */
            /*****************************************************************************/

            TestCase()
                .setInput("mov")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "mov" }),

            TestCase()
                .setInput("lea")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "lea" }),

            
            /*****************************************************************************/
            /*                                 Interrupts                                */
            /*****************************************************************************/

            TestCase()
                .setInput("int")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "int" }),

            TestCase()
                .setInput("syscall")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "syscall" }),


            /*****************************************************************************/
            /*                                     Misc                                  */
            /*****************************************************************************/

            TestCase()
                .setInput("times")
                .setExpectedTokens({ Lexing::TokenType::Instruction })
                .setExpectedStrings({ "times" }),

        };

        check(testCases);
    }

    TEST(Registers, LexerTests)
    {
        TestCases testCases = {
            
            /*****************************************************************************/
            /*                                     RAX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("al")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "al" }),

            TestCase()
                .setInput("ah")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "ah" }),

            TestCase()
                .setInput("ax")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "ax" }),

            TestCase()
                .setInput("eax")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "eax" }),

            TestCase()
                .setInput("rax")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rax" }),


            /*****************************************************************************/
            /*                                     RBX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("bl")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "bl" }),

            TestCase()
                .setInput("bh")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "bh" }),

            TestCase()
                .setInput("bx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "bx" }),

            TestCase()
                .setInput("ebx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "ebx" }),

            TestCase()
                .setInput("rbx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rbx" }),


            /*****************************************************************************/
            /*                                     RCX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("cl")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "cl" }),

            TestCase()
                .setInput("ch")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "ch" }),

            TestCase()
                .setInput("cx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "cx" }),

            TestCase()
                .setInput("ecx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "ecx" }),

            TestCase()
                .setInput("rcx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rcx" }),

            
            /*****************************************************************************/
            /*                                     RDX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("dl")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "dl" }),

            TestCase()
                .setInput("dh")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "dh" }),

            TestCase()
                .setInput("dx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "dx" }),

            TestCase()
                .setInput("edx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "edx" }),

            TestCase()
                .setInput("rdx")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rdx" }),


            /*****************************************************************************/
            /*                                     RSP                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("sp")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "sp" }),

            TestCase()
                .setInput("esp")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "esp" }),

            TestCase()
                .setInput("rsp")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rsp" }),


            /*****************************************************************************/
            /*                                     RBP                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("bp")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "bp" }),

            TestCase()
                .setInput("ebp")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "ebp" }),

            TestCase()
                .setInput("rbp")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rbp" }),


            /*****************************************************************************/
            /*                                     RSI                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("si")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "si" }),

            TestCase()
                .setInput("esi")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "esi" }),

            TestCase()
                .setInput("rsi")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rsi" }),


            /*****************************************************************************/
            /*                                     RDI                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("di")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "di" }),

            TestCase()
                .setInput("edi")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "edi" }),

            TestCase()
                .setInput("rdi")
                .setExpectedTokens({ Lexing::TokenType::Register })
                .setExpectedStrings({ "rdi" }),

        };

        check(testCases);
    }
}
