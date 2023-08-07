#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include "codegen/Elf.h"

#include "error/ErrorReporter.h"

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

    std::unique_ptr<error::IErrorReporter> errorReporter = std::make_unique<error::ErrorReporter>();

    parsing::Parser parser(inPath, tokens, *output, *errorReporter);
    parser.parse();

    std::ofstream outFile(inPath + ".o", std::ios::out | std::ios::binary);
    output->print(outFile);

    return 0;
}