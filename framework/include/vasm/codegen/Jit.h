#ifndef VASM_CODEGEN_JIT_H
#define VASM_CODEGEN_JIT_H 1

#include "vasm/codegen/Binary.h"

#include <memory>
#include <utility>

namespace codegen
{
    template<class>
    class JITCallable;

    template <class T, class... Args>
    class JITCallable<T(Args...)>
    {
    public:
        JITCallable(BinaryFormat* format)
        {
            mBuffer = format->loadjit();
        }

        T operator()(Args&&... args)
        {
            T(*fn)(Args...) = static_cast<T(*)(Args...)>(mBuffer.get());
            return fn(args...);
        }

    private:
        std::unique_ptr<unsigned char[]> mBuffer;
    };
}

#endif // VASM_CODEGEN_JIT_H