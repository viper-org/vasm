#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include "codegen/Elf.h"

#include <sstream>
#include <fstream>
#include <memory>

int main(int argc, char** argv)
{
    std::string inPath = argv[1];
    std::ifstream inFile(inPath);
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string text = buffer.str();

    lexing::Lexer lexer(text);
    std::vector<lexing::Token> tokens = lexer.lex();

    std::unique_ptr<codegen::IOutputFormat> output = std::make_unique<codegen::ELFFormat>(inPath);

    parsing::Parser parser(inPath, tokens, *output);
    parser.parse();

    std::ofstream outFile(inPath + ".o", std::ios::out | std::ios::binary);
    output->print(outFile);

    return 0;
}