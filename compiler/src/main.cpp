#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include "codegen/Elf.h"
#include "codegen/Pe.h"
#include "codegen/Binary.h"

#include "error/ErrorReporter.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "vasm: no input files\n";
        return 1;
    }

    std::optional<std::string> inputFile;
    std::optional<std::string> outputFile;

    std::optional<std::string> outputType;
    std::unique_ptr<codegen::IOutputFormat> outputFormat;

    for (int i = 1; i < argc; i++)
    {
        std::string_view arg = argv[i];
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
                case 'o':
                {
                    if (arg.length() == 2)
                    {
                        outputFile = argv[++i];
                    }
                    else
                    {
                        outputFile = arg.substr(2);
                    }
                    break;
                }
                case 'f':
                {
                    if (arg.length() == 2)
                    {
                        outputType = argv[++i];
                    }
                    else
                    {
                        outputType = arg.substr(2);
                    }
                    break;
                }
                case 'h':
                {
                    std::cout << "Usage: vasm [options...] filename\n\n";
                    std::cout << "Options (values in brackets indicate defaults):\n\n";
                    std::array<std::pair<std::string, std::string>, 3> options = {{
                        { "-h", "show this text" },
                        { "-o outfile", "write output to outfile" },
                        { "-f format", "select output file format" }
                    }};
                    auto padding = std::max_element(options.begin(), options.end(), [](const auto& option1, const auto& option2) {
                        return option1.first.length() < option2.first.length();
                    })->first.length();

                    for (const auto& option : options)
                    {
                        std::cout << "\t" << option.first;
                        std::cout << std::string(padding - option.first.length() + 1, ' ');
                        std::cout << option.second;
                        std::cout << "\n";
                    }
                    return 0;
                }
                default:
                {
                    std::cerr << std::format("vasm: unrecognized command-line option '\x1b[1m{}\x1b[0m'", arg);
                    return 1;
                }
            }
        }
        else
        {
            inputFile = arg;
        }
    }
    
    if (!inputFile)
    {
        std::cerr << "vasm: no input files\n";
        return 1;
    }
    if (!outputFile)
    {
        outputFile = inputFile->substr(0, inputFile->find('.')) + ".o";
    }

    if (!outputType)
    {
        outputFormat = std::make_unique<codegen::BinaryFormat>();
    }
    else if (outputType == "elf")
    {
        outputFormat = std::make_unique<codegen::ELFFormat>(*inputFile);
    }
    else if (outputType == "pe")
    {
        outputFormat = std::make_unique<codegen::PEFormat>(*inputFile);
    }
    else if (outputType == "bin")
    {
        outputFormat = std::make_unique<codegen::BinaryFormat>();
    }
    else
    {
        std::cerr << std::format("vasm: unknown output format: `{}'", *outputType);
        return 1;
    }

    std::string inPath = argv[1];
    std::ifstream inFile(inPath);
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string text = buffer.str();

    lexing::Lexer lexer(text);
    std::vector<lexing::Token> tokens = lexer.lex();

    std::unique_ptr<error::IErrorReporter> errorReporter = std::make_unique<error::ErrorReporter>();

    parsing::Parser parser(inPath, tokens, *outputFormat, *errorReporter);
    parser.parse();

    std::ofstream outFile(*outputFile, std::ios::out | std::ios::binary);
    outputFormat->print(outFile);

    return 0;
}