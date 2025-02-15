// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_SETCC_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_SETCC_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

namespace instruction
{
    template <InstructionStringBuilder Name, OpcodeT auto Opcode>
    struct SetccInstructionImpl;
    template <InstructionStringBuilder Name, auto... Ts>
    using SetccInstruction = SingleOperandInstructionTemplate<SetccInstructionImpl<Name, Ts...>, Name>;


    template <InstructionStringBuilder Name, OpcodeT auto Opcode>
    struct SetccInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, SetccInstruction<Name, Opcode>& instruction)
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

    using SetaInstruction   = SetccInstruction<"seta",   codegen::SETA>;
    using SetaeInstruction  = SetccInstruction<"setae",  codegen::SETAE>;
    using SetbInstruction   = SetccInstruction<"setb",   codegen::SETB>;
    using SetbeInstruction  = SetccInstruction<"setbe",  codegen::SETBE>;
    using SetcInstruction   = SetccInstruction<"setc",   codegen::SETC>;
    using SeteInstruction   = SetccInstruction<"sete",   codegen::SETE>;
    using SetgInstruction   = SetccInstruction<"setg",   codegen::SETG>;
    using SetgeInstruction  = SetccInstruction<"setge",  codegen::SETGE>;
    using SetlInstruction   = SetccInstruction<"setl",   codegen::SETL>;
    using SetleInstruction  = SetccInstruction<"setle",  codegen::SETLE>;
    using SetnaInstruction  = SetccInstruction<"setna",  codegen::SETNA>;
    using SetnaeInstruction = SetccInstruction<"setnae", codegen::SETNAE>;
    using SetnbInstruction  = SetccInstruction<"setnb",  codegen::SETNB>;
    using SetnbeInstruction = SetccInstruction<"setnbe", codegen::SETNBE>;
    using SetncInstruction  = SetccInstruction<"setnc",  codegen::SETNC>;
    using SetneInstruction  = SetccInstruction<"setne",  codegen::SETNE>;
    using SetngInstruction  = SetccInstruction<"setng",  codegen::SETNG>;
    using SetngeInstruction = SetccInstruction<"setnge", codegen::SETNGE>;
    using SetnlInstruction  = SetccInstruction<"setnl",  codegen::SETNL>;
    using SetnleInstruction = SetccInstruction<"setnle", codegen::SETNLE>;
    using SetnoInstruction  = SetccInstruction<"setno",  codegen::SETNO>;
    using SetnpInstruction  = SetccInstruction<"setnp",  codegen::SETNP>;
    using SetnsInstruction  = SetccInstruction<"setns",  codegen::SETNS>;
    using SetnzInstruction  = SetccInstruction<"setnz",  codegen::SETNZ>;
    using SetoInstruction   = SetccInstruction<"seto",   codegen::SETO>;
    using SetpInstruction   = SetccInstruction<"setp",   codegen::SETP>;
    using SetpeInstruction  = SetccInstruction<"setpe",  codegen::SETPE>;
    using SetpoInstruction  = SetccInstruction<"seto",   codegen::SETPO>;
    using SetsInstruction   = SetccInstruction<"sets",   codegen::SETS>;
    using SetzInstruction   = SetccInstruction<"setz",   codegen::SETZ>;
}

#endif // VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_SETCC_H