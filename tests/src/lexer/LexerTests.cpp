#include "Test.h"

#include "vasm/lexer/Lexer.h"
#include "vasm/lexer/Token.h"

namespace LexerTests
{
    struct TestCase
    {
        std::string input;
        std::vector<lexing::TokenType> expectedTokens;
        std::vector<std::string> expectedStrings;

        TestCase& setInput(const    std::string&                          newInput)           { input           = newInput;           return *this; }
        TestCase& setExpectedTokens(const std::vector<lexing::TokenType>& newExpectedTokens)  { expectedTokens  = newExpectedTokens;  return *this; }
        TestCase& setExpectedStrings(const std::vector<std::string>&      newExpectedStrings) { expectedStrings = newExpectedStrings; return *this; }
    };
    using TestCases = std::vector<TestCase>;

    void check(const TestCases& testCases)
    {
        for (const auto& testCase : testCases)
        {
            lexing::Lexer lexer(testCase.input);
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
                .setExpectedTokens({ lexing::TokenType::Dollar })
                .setExpectedStrings({ "" }),
            
            TestCase()
                .setInput("$$")
                .setExpectedTokens({ lexing::TokenType::DollarDollar })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput("$$$")
                .setExpectedTokens({ lexing::TokenType::DollarDollar, lexing::TokenType::Dollar })
                .setExpectedStrings({ "", "" }),

            TestCase()
                .setInput("$ $$")
                .setExpectedTokens({ lexing::TokenType::Dollar, lexing::TokenType::DollarDollar })
                .setExpectedStrings({ "", "" }),

            TestCase()
                .setInput("$$$$")
                .setExpectedTokens({ lexing::TokenType::DollarDollar, lexing::TokenType::DollarDollar })
                .setExpectedStrings({ "", "" }),

        };

        check(testCases);
    }

    TEST(Operators, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("+")
                .setExpectedTokens({ lexing::TokenType::Plus })
                .setExpectedStrings({ "" }),
            TestCase()
                .setInput("-")
                .setExpectedTokens({ lexing::TokenType::Minus })
                .setExpectedStrings({ "" }),
            TestCase()
                .setInput("*")
                .setExpectedTokens({ lexing::TokenType::Star })
                .setExpectedStrings({ "" }),
            TestCase()
                .setInput("/")
                .setExpectedTokens({ lexing::TokenType::Slash })
                .setExpectedStrings({ "" }),

        };

        check(testCases);
    }

    TEST(Brackets, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("(")
                .setExpectedTokens({ lexing::TokenType::LParen })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput(")")
                .setExpectedTokens({ lexing::TokenType::RParen })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput("[")
                .setExpectedTokens({ lexing::TokenType::LBracket })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput("]")
                .setExpectedTokens({ lexing::TokenType::RBracket })
                .setExpectedStrings({ "" }),

        };

        check(testCases);
    }

    TEST(Punctuation, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput(":")
                .setExpectedTokens({ lexing::TokenType::Colon })
                .setExpectedStrings({ "" }),

            TestCase()
                .setInput(",")
                .setExpectedTokens({ lexing::TokenType::Comma })
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
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0" }),

            TestCase()
                .setInput("1")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "1" }),

            TestCase()
                .setInput("1234567890")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "1234567890" }),

            TestCase()
                .setInput("123A")
                .setExpectedTokens({ lexing::TokenType::Immediate, lexing::TokenType::Identifier })
                .setExpectedStrings({ "123", "A" }),

            /*****************************************************************************/
            /*                               Hexadecimal                                 */
            /*****************************************************************************/

            TestCase()
                .setInput("0x0")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0x0" }),

            TestCase()
                .setInput("0x1")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0x1" }),

            TestCase()
                .setInput("0x1234567890ABCDEFabcdef")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0x1234567890ABCDEFabcdef" }),

            TestCase()
                .setInput("0x123G")
                .setExpectedTokens({ lexing::TokenType::Immediate, lexing::TokenType::Identifier })
                .setExpectedStrings({ "0x123", "G" }),

            /*****************************************************************************/
            /*                                   Octal                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("00")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "00" }),

            TestCase()
                .setInput("01")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "01" }),

            TestCase()
                .setInput("012345670")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "012345670" }),

            TestCase()
                .setInput("01238")
                .setExpectedTokens({ lexing::TokenType::Immediate, lexing::TokenType::Immediate })
                .setExpectedStrings({ "0123", "8" }),

            /*****************************************************************************/
            /*                                  Binary                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("0b0")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b0" }),

            TestCase()
                .setInput("0b1")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b1" }),

            TestCase()
                .setInput("0b10")
                .setExpectedTokens({ lexing::TokenType::Immediate })
                .setExpectedStrings({ "0b10" }),

            TestCase()
                .setInput("0b1012")
                .setExpectedTokens({ lexing::TokenType::Immediate, lexing::TokenType::Immediate })
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
                .setExpectedTokens({ lexing::TokenType::String })
                .setExpectedStrings({ "A" }),

            TestCase()
                .setInput("\"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz\"")
                .setExpectedTokens({ lexing::TokenType::String })
                .setExpectedStrings({ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz" }),

            TestCase()
                .setInput("\"!£$%^&*()-=_+[]{};'#:@~,./<>?|`¬\"")
                .setExpectedTokens({ lexing::TokenType::String })
                .setExpectedStrings({ "!£$%^&*()-=_+[]{};'#:@~,./<>?|`¬" }),

            /*****************************************************************************/
            /*                                 Escapes                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("\"" "\\'" "\"")
                .setExpectedTokens({ lexing::TokenType::String })
                .setExpectedStrings({ "'" }),

            TestCase()
                .setInput("\"" "\\n" "\"")
                .setExpectedTokens({ lexing::TokenType::String })
                .setExpectedStrings({ "\n" }),

            TestCase()
                .setInput("\"" "\\\\" "\"")
                .setExpectedTokens({ lexing::TokenType::String })
                .setExpectedStrings({ "\\" }),

        };

        check(testCases);
    }

    TEST(Identifiers, LexerTests)
    {
        TestCases testCases = {

            TestCase()
                .setInput("a")
                .setExpectedTokens({ lexing::TokenType::Identifier })
                .setExpectedStrings({ "a" }),

            TestCase()
                .setInput("_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_")
                .setExpectedTokens({ lexing::TokenType::Identifier })
                .setExpectedStrings({ "_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_" }),
            
            TestCase()
                .setInput("a a")
                .setExpectedTokens({ lexing::TokenType::Identifier, lexing::TokenType::Identifier })
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
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "db" }),

            TestCase()
                .setInput("dw")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "dw" }),

            TestCase()
                .setInput("dd")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "dd" }),
            
            TestCase()
                .setInput("dq")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "dq" }),


            /*****************************************************************************/
            /*                                Branches                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("jmp")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "jmp" }),

            TestCase()
                .setInput("call")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "call" }),

            TestCase()
                .setInput("ret")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "ret" }),

            
            /*****************************************************************************/
            /*                                    Loads                                  */
            /*****************************************************************************/

            TestCase()
                .setInput("mov")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "mov" }),

            TestCase()
                .setInput("lea")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "lea" }),

            
            /*****************************************************************************/
            /*                                 Interrupts                                */
            /*****************************************************************************/

            TestCase()
                .setInput("int")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "int" }),

            TestCase()
                .setInput("syscall")
                .setExpectedTokens({ lexing::TokenType::Instruction })
                .setExpectedStrings({ "syscall" }),


            /*****************************************************************************/
            /*                                     Misc                                  */
            /*****************************************************************************/

            TestCase()
                .setInput("times")
                .setExpectedTokens({ lexing::TokenType::Instruction })
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
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "al" }),

            TestCase()
                .setInput("ah")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "ah" }),

            TestCase()
                .setInput("ax")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "ax" }),

            TestCase()
                .setInput("eax")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "eax" }),

            TestCase()
                .setInput("rax")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rax" }),


            /*****************************************************************************/
            /*                                     RBX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("bl")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "bl" }),

            TestCase()
                .setInput("bh")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "bh" }),

            TestCase()
                .setInput("bx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "bx" }),

            TestCase()
                .setInput("ebx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "ebx" }),

            TestCase()
                .setInput("rbx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rbx" }),


            /*****************************************************************************/
            /*                                     RCX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("cl")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "cl" }),

            TestCase()
                .setInput("ch")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "ch" }),

            TestCase()
                .setInput("cx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "cx" }),

            TestCase()
                .setInput("ecx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "ecx" }),

            TestCase()
                .setInput("rcx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rcx" }),

            
            /*****************************************************************************/
            /*                                     RDX                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("dl")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "dl" }),

            TestCase()
                .setInput("dh")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "dh" }),

            TestCase()
                .setInput("dx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "dx" }),

            TestCase()
                .setInput("edx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "edx" }),

            TestCase()
                .setInput("rdx")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rdx" }),


            /*****************************************************************************/
            /*                                     RSP                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("sp")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "sp" }),

            TestCase()
                .setInput("esp")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "esp" }),

            TestCase()
                .setInput("rsp")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rsp" }),


            /*****************************************************************************/
            /*                                     RBP                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("bp")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "bp" }),

            TestCase()
                .setInput("ebp")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "ebp" }),

            TestCase()
                .setInput("rbp")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rbp" }),


            /*****************************************************************************/
            /*                                     RSI                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("si")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "si" }),

            TestCase()
                .setInput("esi")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "esi" }),

            TestCase()
                .setInput("rsi")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rsi" }),


            /*****************************************************************************/
            /*                                     RDI                                   */
            /*****************************************************************************/

            TestCase()
                .setInput("di")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "di" }),

            TestCase()
                .setInput("edi")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "edi" }),

            TestCase()
                .setInput("rdi")
                .setExpectedTokens({ lexing::TokenType::Register })
                .setExpectedStrings({ "rdi" }),

        };

        check(testCases);
    }
}
