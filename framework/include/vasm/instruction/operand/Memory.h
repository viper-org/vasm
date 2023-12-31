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
        Memory(RegisterPtr reg, std::optional<int> displacement);

        Register* getReg() const;
        std::optional<int> getDisplacement() const;
        codegen::AddressingMode getAddressingMode() const;

        std::unique_ptr<Operand> clone() override;

    private:
        RegisterPtr mReg;
        std::optional<int> mDisplacement;
    };
    using MemoryPtr = std::unique_ptr<Memory>;
}

#endif