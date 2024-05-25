// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_SETCC_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_SETCC_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

namespace instruction
{
    template <OpcodeT auto Opcode>
    struct SetccInstructionImpl;
    template <auto... Ts>
    using SetccInstruction = SingleOperandInstructionTemplate<SetccInstructionImpl<Ts...>>;


    template <OpcodeT auto Opcode>
    struct SetccInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, SetccInstruction<Opcode>& instruction)
        {
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            codegen::REX rex = codegen::REX::None;
            std::optional<int> displacement;

            Register* reg = dynamic_cast<Register*>(instruction.getOperand().get());
            if (!reg)
            {
                if (auto mem = dynamic_cast<Memory*>(instruction.getOperand().get()))
                {
                    reg = mem->getBase();
                    addressingMode = mem->getAddressingMode();
                    displacement = mem->getDisplacement();
                    sib = mem->getSIB();
                    rex = mem->getRex();
                }
            }
            else
            {
                rex = reg->getRex();
            }

            builder.createInstruction(section)
                .prefix(rex)
                .opcode(Opcode)
                .modrm(addressingMode, 0, reg->getID())
                .sib(sib)
                .displacement(displacement)
                .emit();
        }
    };

    using SetaInstruction = SetccInstruction<codegen::SETA>;
    using SetaeInstruction = SetccInstruction<codegen::SETAE>;
    using SetbInstruction = SetccInstruction<codegen::SETB>;
    using SetbeInstruction = SetccInstruction<codegen::SETBE>;
    using SetcInstruction = SetccInstruction<codegen::SETC>;
    using SeteInstruction = SetccInstruction<codegen::SETE>;
    using SetgInstruction = SetccInstruction<codegen::SETG>;
    using SetgeInstruction = SetccInstruction<codegen::SETGE>;
    using SetlInstruction = SetccInstruction<codegen::SETL>;
    using SetleInstruction = SetccInstruction<codegen::SETLE>;
    using SetnaInstruction = SetccInstruction<codegen::SETNA>;
    using SetnaeInstruction = SetccInstruction<codegen::SETNAE>;
    using SetnbInstruction = SetccInstruction<codegen::SETNB>;
    using SetnbeInstruction = SetccInstruction<codegen::SETNBE>;
    using SetncInstruction = SetccInstruction<codegen::SETNC>;
    using SetneInstruction = SetccInstruction<codegen::SETNE>;
    using SetngInstruction = SetccInstruction<codegen::SETNG>;
    using SetngeInstruction = SetccInstruction<codegen::SETNGE>;
    using SetnlInstruction = SetccInstruction<codegen::SETNL>;
    using SetnleInstruction = SetccInstruction<codegen::SETNLE>;
    using SetnoInstruction = SetccInstruction<codegen::SETNO>;
    using SetnpInstruction = SetccInstruction<codegen::SETNP>;
    using SetnsInstruction = SetccInstruction<codegen::SETNS>;
    using SetnzInstruction = SetccInstruction<codegen::SETNZ>;
    using SetoInstruction = SetccInstruction<codegen::SETO>;
    using SetpInstruction = SetccInstruction<codegen::SETP>;
    using SetpeInstruction = SetccInstruction<codegen::SETPE>;
    using SetpoInstruction = SetccInstruction<codegen::SETPO>;
    using SetsInstruction = SetccInstruction<codegen::SETS>;
    using SetzInstruction = SetccInstruction<codegen::SETZ>;
}

#endif // VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_SETCC_H