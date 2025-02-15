// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_OPERAND_MEMORY_H
#define VASM_INSTRUCTION_OPERAND_MEMORY_H 1

#include "vasm/instruction/Operand.h"

#include "vasm/codegen/builder/ModRM.h"

#include <optional>

namespace instruction
{
    class Register;
    using RegisterPtr = std::unique_ptr<Register>;

    class Memory : public Operand
    {
    public:
        Memory(RegisterPtr base, std::optional<int> displacement, RegisterPtr index, std::optional<int> scale);

        Register* getBase() const;
        Register* getIndex() const;
        std::optional<int> getDisplacement() const;
        std::optional<int> getScale() const;
        codegen::AddressingMode getAddressingMode() const;

        codegen::SIB getSIB() const;
        codegen::REX getRex() const;

        std::unique_ptr<Operand> clone() override;

        std::string toString() override;

    private:
        RegisterPtr mBase;
        std::optional<int> mDisplacement;
        RegisterPtr mIndex;
        std::optional<int> mScale;
    };
    using MemoryPtr = std::unique_ptr<Memory>;
}

#endif