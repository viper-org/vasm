cmake_minimum_required (VERSION 3.26)

project (vasm)

include_directories(include)
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vasm_sources
    src/main.cpp

    src/lexer/Lexer.cpp
    src/lexer/Token.cpp

    src/parser/Parser.cpp

    src/codegen/Binary.cpp
    src/codegen/Elf.cpp
)

add_executable(vasm ${vasm_sources})

target_compile_features(vasm PUBLIC cxx_std_20)