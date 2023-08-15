// Copyright 2023 solar-mist

#ifndef VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JCC_H
#define VASM_INSTRUCTION_SINGLE_OPERAND_INSTRUCTION_JCC_H 1

#include "vasm/instruction/SingleOperandInstruction.h"

#include "vasm/instruction/operand/Label.h"

namespace instruction
{
    template <OpcodeT auto Opcode>
    struct JccInstructionImpl;
    template <auto... Ts>
    using JccInstruction = SingleOperandInstructionTemplate<JccInstructionImpl<Ts...>>;


    template <OpcodeT auto Opcode>
    struct JccInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, JccInstruction<Opcode>& instruction)
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
                    if (value.second)
                    {
                        label->reloc(builder, section, -6);
                    }
                }
                else
                {
                    builder.createInstruction(section)
                        .opcode(Opcode)
                        .immediate(static_cast<unsigned char>(value.first - builder.getPosition(section) - 2))
                        .emit();
                    if (value.second)
                    {
                        label->reloc(builder, section, -2);
                    }
                }
            }
        }
    };

    using JaInstruction    = JccInstruction<codegen::JA_REL8>;
    using JaeInstruction   = JccInstruction<codegen::JAE_REL8>;
    using JbInstruction    = JccInstruction<codegen::JB_REL8>;
    using JbeInstruction   = JccInstruction<codegen::JBE_REL8>;
    using JcxzInstruction  = JccInstruction<codegen::JCXZ_REL8>;
    using JecxzInstruction = JccInstruction<codegen::JECXZ_REL8>;
    using JrcxzInstruction = JccInstruction<codegen::JRCXZ_REL8>;
    using JeInstruction    = JccInstruction<codegen::JE_REL8>;
    using JgInstruction    = JccInstruction<codegen::JG_REL8>;
    using JgeInstruction   = JccInstruction<codegen::JGE_REL8>;
    using JlInstruction    = JccInstruction<codegen::JL_REL8>;
    using JleInstruction   = JccInstruction<codegen::JLE_REL8>;
    using JnaInstruction   = JccInstruction<codegen::JNA_REL8>;
    using JnaeInstruction  = JccInstruction<codegen::JNAE_REL8>;
    using JnbInstruction   = JccInstruction<codegen::JNB_REL8>;
    using JnbeInstruction  = JccInstruction<codegen::JNBE_REL8>;
    using JncInstruction   = JccInstruction<codegen::JNC_REL8>;
    using JneInstruction   = JccInstruction<codegen::JNE_REL8>;
    using JngInstruction   = JccInstruction<codegen::JNG_REL8>;
    using JngeInstruction  = JccInstruction<codegen::JNGE_REL8>;
    using JnlInstruction   = JccInstruction<codegen::JNL_REL8>;
    using JnleInstruction  = JccInstruction<codegen::JNLE_REL8>;
    using JnoInstruction   = JccInstruction<codegen::JNO_REL8>;
    using JnpInstruction   = JccInstruction<codegen::JNP_REL8>;
    using JnsInstruction   = JccInstruction<codegen::JNS_REL8>;
    using JnzInstruction   = JccInstruction<codegen::JNZ_REL8>;
    using JoInstruction    = JccInstruction<codegen::JO_REL8>;
    using JpInstruction    = JccInstruction<codegen::JP_REL8>;
    using JpeInstruction   = JccInstruction<codegen::JPE_REL8>;
    using JpoInstruction   = JccInstruction<codegen::JPO_REL8>;
    using JsInstruction    = JccInstruction<codegen::JS_REL8>;
    using JzInstruction    = JccInstruction<codegen::JZ_REL8>;
}

#endif