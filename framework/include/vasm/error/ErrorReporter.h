#ifndef VASM_ERROR_ERROR_REPORTER_H
#define VASM_ERROR_ERROR_REPORTER_H 1

#include "vasm/error/IErrorReporter.h"

namespace error
{
    class ErrorReporter : public IErrorReporter
    {
    public:
        ErrorReporter();

        void reportError(ErrorContext errorContext) const override;
    };
}

#endif