cmake_minimum_required (VERSION 3.26)

project (vasm_fakes_project)

include_directories(${vasm_framework_project_include_dirs})
include_directories(include)
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vasm_fakes_project_sources
    src/codegen/FakeOutputFormat.cpp
)

add_library(vasm_fakes_project SHARED ${vasm_fakes_project_sources})
set_target_properties(vasm_fakes_project PROPERTIES OUTPUT_NAME "vasm_fakes")

target_compile_features(vasm_fakes_project PUBLIC cxx_std_20)
target_link_libraries(vasm_fakes_project vasm_framework_project)

set(${PROJECT_NAME}_include_dirs ${PROJECT_SOURCE_DIR}/include ${vasm_framework_project_include_dirs} CACHE INTERNAL "${PROJECT_NAME} Include Directories" FORCE)