#ifndef VASM_ERROR_IERROR_REPORTER_H
#define VASM_ERROR_IERROR_REPORTER_H 1

#include <memory>
#include <string>

namespace lexing
{
    class Token;
}

namespace error
{
    struct ErrorContext
    {
        std::string_view file;
        std::string_view message;
        lexing::Token& token;
    };

    class IErrorReporter
    {
    public:
        virtual ~IErrorReporter() { }

        virtual void reportError(ErrorContext errorContext) const = 0;
    };
}

#endif