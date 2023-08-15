cmake_minimum_required (VERSION 3.26)

project (vasm_project)

include_directories(${vasm_framework_project_include_dirs})
include_directories(include)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vasm_project_sources
    src/main.cpp
)

add_executable(vasm_project ${vasm_project_sources})
set_target_properties(vasm_project PROPERTIES OUTPUT_NAME "vasm")

target_compile_features(vasm_project PUBLIC cxx_std_20)
target_link_libraries(vasm_project vasm_framework_project)