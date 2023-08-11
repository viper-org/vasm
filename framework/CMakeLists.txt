cmake_minimum_required (VERSION 3.26)

project (vasm_framework_project)

include_directories(include)
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vasm_framework_project_sources
    src/main.cpp


    src/lexer/Lexer.cpp
    src/lexer/Token.cpp


    src/parser/Parser.cpp
    src/parser/NewParser.cpp


    src/codegen/Binary.cpp
    src/codegen/Elf.cpp
	src/codegen/Pe.cpp

    src/codegen/builder/OpcodeBuilder.cpp
    src/codegen/builder/Instruction.cpp
    src/codegen/builder/ModRM.cpp


    src/instruction/operand/Immediate.cpp
    src/instruction/operand/Register.cpp

    src/instruction/noOperandInstruction/RetInstruction.cpp
    src/instruction/noOperandInstruction/SyscallInstruction.cpp

    src/instruction/singleOperandInstruction/PushInstruction.cpp
    src/instruction/singleOperandInstruction/PopInstruction.cpp

    src/instruction/twoOperandInstruction/MovInstruction.cpp


    src/error/ErrorReporter.cpp
)

add_library(vasm_framework_project SHARED ${vasm_framework_project_sources})
set_target_properties(vasm_framework_project PROPERTIES OUTPUT_NAME "vasm_framework")

target_compile_features(vasm_framework_project PUBLIC cxx_std_20)

set(${PROJECT_NAME}_include_dirs ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "${PROJECT_NAME} Include Directories" FORCE)