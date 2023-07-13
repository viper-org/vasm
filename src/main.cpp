#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <parser/Parser.h>

#include <codegen/Binary.h>

#include <iostream>
#include <fstream>
#include <string>
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

    std::unique_ptr<Codegen::OutputFormat> output = std::make_unique<Codegen::BinaryFormat>();

    Parsing::Parser parser(tokens, *output);
    parser.parse();

    //output->write((short)0xFEEB, Codegen::Section::Text);
    //for(int i = 0; i < 508; i++)
    //    output->write((char)0x00, Codegen::Section::Text);
    //output->write((short)0xAA55, Codegen::Section::Text);

    std::ofstream outFile(inPath + ".o", std::ios::out | std::ios::binary);
    output->print(outFile);
    return 0;
}