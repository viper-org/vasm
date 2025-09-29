// Copyright 2023 solar-mist


#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"

#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"

#include "vasm/codegen/Opcodes.h"

namespace instruction
{
    void CallInstructionImpl::emit(codegen::OpcodeBuilder& builder, codegen::Section section, CallInstruction& instruction)
    {
        if (LabelOperand* label = dynamic_cast<LabelOperand*>(instruction.getOperand().get()))
        {
            auto value = label->getValue(builder, section);
            uint32_t trueValue = value.first - builder.getPosition(section) - 5;
            if (value.second || value.first == -1) trueValue = 0;

            builder.createInstruction(section)
                   .opcode(codegen::CALL_REL32)
                   .immediate(trueValue)
                   .emit();
            if (value.second || value.first == -1)
            {
                label->reloc(builder, section, codegen::OperandSize::Long, -4);
            }
        }
        else
        {
            codegen::AddressingMode addressingMode = codegen::AddressingMode::RegisterDirect;
            codegen::SIB sib;
            std::optional<int> displacement;
            codegen::OperandSize size = instruction.getSize();

            Register* reg = dynamic_cast<Register*>(instruction.getOperand().get());
            Memory*   mem = dynamic_cast<Memory*>(instruction.getOperand().get());
            if (mem)
            {
                addressingMode = mem->getAddressingMode();
                displacement = mem->getDisplacement();
                reg = mem->getBase();
                sib = mem->getSIB();
            }
            else
            {
                size = reg->getSize();
            }

            switch(size)
            {
                case codegen::OperandSize::Byte:
                    break; // TODO: Error
                case codegen::OperandSize::Word:
                    builder.createInstruction(section)
                        .opcode(codegen::CALL_RM16)
                        .modrm(addressingMode, 2, reg->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Long:
                    builder.createInstruction(section)
                        .opcode(codegen::CALL_RM32)
                        .modrm(addressingMode, 2, reg->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::Quad:
                    builder.createInstruction(section)
                        .opcode(codegen::CALL_RM64)
                        .modrm(addressingMode, 2, reg->getID())
                        .displacement(displacement)
                        .emit();
                    break;
                case codegen::OperandSize::None:
                    break;
            }
        }
    }
}