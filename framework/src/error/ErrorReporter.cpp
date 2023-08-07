#include "error/ErrorReporter.h"

#include "lexer/Token.h"

#include <format>
#include <iostream>

namespace error
{
    ErrorReporter::ErrorReporter()
    {
    }

    void ErrorReporter::reportError(ErrorContext errorContext) const
    {
        std::cerr << std::format("{}:{}:{}: {}\n", errorContext.file, errorContext.token.getSourceLocation().line, errorContext.token.getSourceLocation().column, errorContext.message);
        std::exit(1);
    }
}