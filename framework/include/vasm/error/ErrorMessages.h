#ifndef VASM_ERROR_ERROR_MESSAGES_H
#define VASM_ERROR_ERROR_MESSAGES_H 1

#include <string>

namespace error
{
    constexpr const std::string_view INV_OPCODE_OPERAND = "Invalid combination of opcodes and operands";
}

#endif