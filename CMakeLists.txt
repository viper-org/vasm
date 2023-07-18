cmake_minimum_required(VERSION 3.26)
project(vasm)

set(CMAKE_BUILD_TYPE Debug)

set(vasm_sources
    src/main.cpp

    src/lexer/Lexer.cpp
    src/lexer/Token.cpp

    src/parser/Parser.cpp

    src/codegen/Binary.cpp
    src/codegen/Elf.cpp
)

add_executable(vasm ${vasm_sources})

target_include_directories(vasm PRIVATE include)
target_compile_options(vasm PRIVATE -fsanitize=address,undefined -Wall -Wextra)
target_link_options(vasm PRIVATE -fsanitize=address,undefined)

target_compile_features(vasm PUBLIC cxx_std_20)