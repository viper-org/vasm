#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <parser/Parser.h>

#include <codegen/Binary.h>
#include <codegen/Elf.h>

#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return EXIT_FAILURE;
    }
    
    std::string inPath = argv[1];
    std::ifstream inFile(inPath);
    if (!inFile) {
        std::cerr << "Error: file '" << inPath << "' was not found\n";
        return EXIT_FAILURE; 
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string text = buffer.str();

    Lexing::Lexer lexer(text);
    std::vector<Lexing::Token> tokens = lexer.lex();

    std::unique_ptr<Codegen::OutputFormat> output = std::make_unique<Codegen::ELFFormat>(inPath);

    Parsing::Parser parser(inPath, tokens, *output);
    parser.parse();

    std::ofstream outFile(inPath + ".o", std::ios::out | std::ios::binary);
    output->print(outFile);

    return 0;
}