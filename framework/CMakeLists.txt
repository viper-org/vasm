cmake_minimum_required (VERSION 3.26)

set(SOURCES
    "src/lexer/Lexer.cpp"
    "src/lexer/Token.cpp"


    "src/parser/Parser.cpp"


    "src/codegen/Binary.cpp"
    "src/codegen/Elf.cpp"
	"src/codegen/Pe.cpp"

    "src/codegen/builder/OpcodeBuilder.cpp"
    "src/codegen/builder/Instruction.cpp"
    "src/codegen/builder/ModRM.cpp"

    
    "src/instruction/operand/Immediate.cpp"
    "src/instruction/operand/Label.cpp"
    "src/instruction/operand/Register.cpp"
    "src/instruction/operand/Memory.cpp"
    "src/instruction/operand/Relative.cpp"
    "src/instruction/operand/String.cpp"
    
    "src/instruction/Label.cpp"
    "src/instruction/Directive.cpp"

    "src/instruction/singleOperandInstruction/CallInstruction.cpp"
    "src/instruction/singleOperandInstruction/JmpInstruction.cpp"
    "src/instruction/singleOperandInstruction/PushInstruction.cpp"
    "src/instruction/singleOperandInstruction/PopInstruction.cpp"
    "src/instruction/singleOperandInstruction/IntInstruction.cpp"

    "src/instruction/twoOperandInstruction/MovInstruction.cpp"
    "src/instruction/twoOperandInstruction/LeaInstruction.cpp"
    "src/instruction/twoOperandInstruction/XchgInstruction.cpp"
    "src/instruction/twoOperandInstruction/TestInstruction.cpp"

    "src/instruction/variableOperandInstruction/IMulInstruction.cpp"


    "src/error/ErrorReporter.cpp"
)

set(HEADERS
    "include/vasm/lexer/Lexer.h"
    "include/vasm/lexer/Token.h"


    "include/vasm/parser/Parser.h"


    "include/vasm/codegen/IOutputFormat.h"
    "include/vasm/codegen/Opcodes.h"
    "include/vasm/codegen/Binary.h"
    "include/vasm/codegen/Elf.h"
	"include/vasm/codegen/Pe.h"

    "include/vasm/codegen/builder/OpcodeBuilder.h"
    "include/vasm/codegen/builder/Instruction.h"
    "include/vasm/codegen/builder/ModRM.h"

    
    "include/vasm/instruction/Value.h"
    "include/vasm/instruction/Instruction.h"
    "include/vasm/instruction/Label.h"
    "include/vasm/instruction/Directive.h"

    "include/vasm/instruction/Operand.h"
    "include/vasm/instruction/operand/Immediate.h"
    "include/vasm/instruction/operand/Label.h"
    "include/vasm/instruction/operand/Register.h"
    "include/vasm/instruction/operand/Memory.h"
    "include/vasm/instruction/operand/Relative.h"
    "include/vasm/instruction/operand/String.h"

    "include/vasm/instruction/NoOperandInstruction.h"
    
    "include/vasm/instruction/singleOperandInstruction/CallInstruction.h"
    "include/vasm/instruction/singleOperandInstruction/JmpInstruction.h"
    "include/vasm/instruction/singleOperandInstruction/PushInstruction.h"
    "include/vasm/instruction/singleOperandInstruction/PopInstruction.h"
    "include/vasm/instruction/singleOperandInstruction/IntInstruction.h"
    "include/vasm/instruction/singleOperandInstruction/Grp4Instruction.h"
    "include/vasm/instruction/singleOperandInstruction/SetccInstruction.h"

    "include/vasm/instruction/twoOperandInstruction/MovInstruction.h"
    "include/vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
    "include/vasm/instruction/twoOperandInstruction/LeaInstruction.h"
    "include/vasm/instruction/twoOperandInstruction/OutInInstruction.h"
    "include/vasm/instruction/twoOperandInstruction/ShiftRotateInstruction.h"
    "include/vasm/instruction/twoOperandInstruction/XchgInstruction.h"
    "include/vasm/instruction/twoOperandInstruction/TestInstruction.h"

    "include/vasm/instruction/variableOperandInstruction/IMulInstruction.h"


    "include/vasm/error/ErrorReporter.h"
    "include/vasm/error/IErrorReporter.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(vasm-framework-vasm-framework ${SOURCES} ${HEADERS})
add_library(vasm::framework ALIAS vasm-framework-vasm-framework)
target_include_directories(vasm-framework-vasm-framework
    PUBLIC
        include
)
target_compile_features(vasm-framework-vasm-framework PUBLIC cxx_std_20)