// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JCC_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JCC_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    template <InstructionStringBuilder Name, OpcodeT auto Opcode>
    struct JccInstructionImpl;
    template <InstructionStringBuilder Name, auto... Ts>
    using JccInstruction = SingleOperandInstructionTemplate<JccInstructionImpl<Name, Ts...>, Name>;


    template <InstructionStringBuilder Name, OpcodeT auto Opcode>
    struct JccInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, JccInstruction<Name, Opcode>& instruction)
        {
            if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getOperand().get()))
            {
                auto value = label->getValue(builder, section);
                if ((value.first - builder.getPosition(section)) > UINT8_MAX)
                {
                    builder.createInstruction(section)
                        .opcode(static_cast<codegen::WordOpcodes>(Opcode + 0x10))
                        .immediate(static_cast<unsigned int>(value.first - builder.getPosition(section) - 6))
                        .emit();
                    if (value.second || value.first == -1)
                    {
                        label->reloc(builder, section, codegen::OperandSize::Long, -4);
                    }
                }
                else
                {
                    builder.createInstruction(section)
                        .opcode(Opcode)
                        .immediate(static_cast<unsigned char>(value.first - builder.getPosition(section) - 2))
                        .emit();
                    if (value.second || value.first == -1)
                    {
                        label->reloc(builder, section, codegen::OperandSize::Byte, -1);
                    }
                }
            }
        }
    };

    using JaInstruction    = JccInstruction<"ja",    codegen::JA_REL8>;
    using JaeInstruction   = JccInstruction<"jae",   codegen::JAE_REL8>;
    using JbInstruction    = JccInstruction<"jb",    codegen::JB_REL8>;
    using JbeInstruction   = JccInstruction<"jbe",   codegen::JBE_REL8>;
    using JcxzInstruction  = JccInstruction<"jcxz",  codegen::JCXZ_REL8>;
    using JecxzInstruction = JccInstruction<"jecxz", codegen::JECXZ_REL8>;
    using JrcxzInstruction = JccInstruction<"jrcxz", codegen::JRCXZ_REL8>;
    using JeInstruction    = JccInstruction<"je",    codegen::JE_REL8>;
    using JgInstruction    = JccInstruction<"jg",    codegen::JG_REL8>;
    using JgeInstruction   = JccInstruction<"jge",   codegen::JGE_REL8>;
    using JlInstruction    = JccInstruction<"jl",    codegen::JL_REL8>;
    using JleInstruction   = JccInstruction<"jle",   codegen::JLE_REL8>;
    using JnaInstruction   = JccInstruction<"jna",   codegen::JNA_REL8>;
    using JnaeInstruction  = JccInstruction<"jnae",  codegen::JNAE_REL8>;
    using JnbInstruction   = JccInstruction<"jnb",   codegen::JNB_REL8>;
    using JnbeInstruction  = JccInstruction<"jnbe",  codegen::JNBE_REL8>;
    using JncInstruction   = JccInstruction<"jnc",   codegen::JNC_REL8>;
    using JneInstruction   = JccInstruction<"jne",   codegen::JNE_REL8>;
    using JngInstruction   = JccInstruction<"jng",   codegen::JNG_REL8>;
    using JngeInstruction  = JccInstruction<"jnge",  codegen::JNGE_REL8>;
    using JnlInstruction   = JccInstruction<"jnl",   codegen::JNL_REL8>;
    using JnleInstruction  = JccInstruction<"jnle",  codegen::JNLE_REL8>;
    using JnoInstruction   = JccInstruction<"jno",   codegen::JNO_REL8>;
    using JnpInstruction   = JccInstruction<"jnp",   codegen::JNP_REL8>;
    using JnsInstruction   = JccInstruction<"jns",   codegen::JNS_REL8>;
    using JnzInstruction   = JccInstruction<"jnz",   codegen::JNZ_REL8>;
    using JoInstruction    = JccInstruction<"jo",    codegen::JO_REL8>;
    using JpInstruction    = JccInstruction<"jp",    codegen::JP_REL8>;
    using JpeInstruction   = JccInstruction<"jpe",   codegen::JPE_REL8>;
    using JpoInstruction   = JccInstruction<"jpo",   codegen::JPO_REL8>;
    using JsInstruction    = JccInstruction<"js",    codegen::JS_REL8>;
    using JzInstruction    = JccInstruction<"jz",    codegen::JZ_REL8>;
}

#endif