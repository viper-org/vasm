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
        Register(unsigned char id, codegen::OperandSize size, bool rex = false, bool extended = false);

        unsigned char getID() const;
        codegen::OperandSize getSize() const;
        bool isExtended() const;

        std::unique_ptr<Operand> clone() override;
        std::unique_ptr<Register> clone(codegen::OperandSize newSize);

        std::string toString() override;

        codegen::REX getRex() const;

        static RegisterPtr Get(std::string_view name);

    private:
        unsigned char mID;
        codegen::OperandSize mSize;
        bool mRex;
        bool mExtended;
    };
}

#endif