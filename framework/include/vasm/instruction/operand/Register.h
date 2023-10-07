// Copyright 2023 solar-mist

#include <memory>
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
        Register(unsigned char id, codegen::OperandSize size);

        unsigned char getID() const;
        codegen::OperandSize getSize() const;

        std::unique_ptr<Operand> clone() override;
        std::unique_ptr<Register> clone(codegen::OperandSize newSize);

        static RegisterPtr Get(std::string_view name);

    private:
        unsigned char mID;
        codegen::OperandSize mSize;
    };
}

#endif