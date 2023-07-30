cmake_minimum_required (VERSION 3.26)

project (vasm_tests_project)

include_directories(${vasm_framework_project_include_dirs})
include_directories(include)
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vasm_tests_project_sources
    src/main.cpp
    src/Test.cpp

    src/lexer/LexerTests.cpp
)

add_executable(vasm_tests_project ${vasm_tests_project_sources})
set_target_properties(vasm_tests_project PROPERTIES OUTPUT_NAME "vasm_tests")

target_compile_features(vasm_tests_project PUBLIC cxx_std_20)
target_link_libraries(vasm_tests_project vasm_framework_project)