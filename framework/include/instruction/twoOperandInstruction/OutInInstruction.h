// Copyright 2023 solar-mist


#include "codegen/Opcodes.h"
#ifndef VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_OUT_IN_H
#define VASM_INSTRUCTION_TWO_OPERAND_INSTRUCTION_OUT_IN_H 1

#include "instruction/TwoOperandInstruction.h"

#include "instruction/operand/Register.h"

#include <cassert>

namespace instruction
{
    template <OpcodeT auto Imm8AL, OpcodeT auto Imm8AX, OpcodeT auto DXAL, OpcodeT auto DXEAX>
    struct InOutInstructionImpl;
    template <auto... Ts>
    using InOutInstruction = TwoOperandInstructionTemplate<InOutInstructionImpl<Ts...>>;

    template <OpcodeT auto Imm8AL, OpcodeT auto Imm8AX, OpcodeT auto DXAL, OpcodeT auto DXEAX>
    struct InOutInstructionImpl
    {
        static void emit(codegen::OpcodeBuilder& builder, codegen::Section section, InOutInstruction<Imm8AL, Imm8AX, DXAL, DXEAX>& instruction)
        {
            if (Register* lhs = dynamic_cast<Register*>(instruction.getLeft().get()))
            {
                assert(lhs->getID() == 2); // DX

                Register* rhs = static_cast<Register*>(instruction.getRight().get());
                assert(rhs->getID() == 0); // AX

                switch (rhs->getSize())
                {
                    case codegen::OperandSize::Byte:
                        builder.createInstruction(section)
                               .opcode(DXAL)
                               .emit();
                        break;
                    case codegen::OperandSize::Word:
                        builder.createInstruction(section)
                               .prefix(codegen::SIZE_PREFIX)
                               .opcode(DXEAX)
                               .emit();
                        break;
                    case codegen::OperandSize::Long:
                        builder.createInstruction(section)
                               .opcode(DXEAX)
                               .emit();
                        break;
                    case codegen::OperandSize::Quad: // TODO: Error
                        break;
                }
            }
        }
    };

    using OutInstruction = InOutInstruction<codegen::OUT_IMM8_AL, codegen::OUT_IMM8_AX, codegen::OUT_DX_AL, codegen::OUT_DX_AX>;
    using InInstruction  = InOutInstruction<codegen::IN_AL_IMM8,  codegen::IN_AX_IMM8,  codegen::IN_AL_DX,  codegen::IN_AX_DX>;
}

#endif