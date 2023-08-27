// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_REGISTER_H
#define VASM_INSTRUCTION_OPERAND_REGISTER_H 1

#include "vasm/instruction/Operand.h"

#include "vasm/codegen/Opcodes.h"

namespace instruction
{
    class Register;
    using RegisterPtr = std::unique_ptr<Register>;

    class Register : public Operand
    {
    public:
        Register(std::uint8_t id, codegen::OperandSize size);

        std::uint8_t getID() const;
        codegen::OperandSize getSize() const;

        std::unique_ptr<Operand> clone() override;

        static RegisterPtr Get(std::string_view name);

    private:
        std::uint8_t mID;
        codegen::OperandSize mSize;
    };
}

#endif