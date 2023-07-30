#include <iostream>
#ifndef VASM_TESTS_ASSERT_H
#define VASM_TESTS_ASSERT_H 1

#include <string>
#include <source_location>

enum class AssertOperator {
    None = 0,
    EQ,
    NEQ,
    LE,
    LT,
    GE,
    GT,
};

enum class AssertResult {
    None,
    True,
    False,
};

struct AssertInfo {
    const char* expr;
    std::source_location sourceLocation;
    AssertOperator op;
    AssertResult result;
    std::string lhs;
    std::string rhs;
};

namespace Lexing
{
    enum class TokenType;
    std::string to_string(const Lexing::TokenType& tokenType);
}

template <class A>
struct AssertCheck {
    A a;
    AssertInfo& i;
    template <class B>
    AssertInfo operator==(B&& b) &&
    {
        using std::to_string;
        i.result = (a == b) ? AssertResult::True : AssertResult::False;
        
        if constexpr(!std::is_same_v<A, const std::string&> && !std::is_same_v<A, std::string&> && !std::is_same_v<A, std::string>)
        {
            i.lhs = to_string(a);
        }
        else
        {
            i.lhs = a;
        }
        if constexpr(!std::is_same_v<B, const std::string&> && !std::is_same_v<B, std::string&> && !std::is_same_v<B, std::string>)
        {
            i.rhs = to_string(b);
        }
        else
        {
            i.rhs = b;
        }

        i.op = AssertOperator::EQ;
        return std::move(i);
    }
};

struct AssertDecomposer {
    AssertInfo i;
    template <class A>
    AssertCheck<A> operator<(A&& a) && {
        return AssertCheck<A>{std::forward<A>(a), i};
    }
};

#endif // VASM_TESTS_ASSERT_H