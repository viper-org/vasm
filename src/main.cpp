#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <parser/Parser.h>

#include <codegen/Binary.h>
#include <codegen/Elf.h>

#include <fstream>
#include <memory>

int main(int argc, char** argv)
{
    std::string inPath = argv[1];
    std::ifstream inFile(inPath);
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string text = buffer.str();

    Lexing::Lexer lexer(text);
    std::vector<Lexing::Token> tokens = lexer.lex();

    std::unique_ptr<Codegen::OutputFormat> output = std::make_unique<Codegen::ELFFormat>(inPath);

    Parsing::Parser parser(tokens, *output);
    parser.parse();

    std::ofstream outFile(inPath + ".o", std::ios::out | std::ios::binary);
    output->print(outFile);

    return 0;
}