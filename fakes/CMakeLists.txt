cmake_minimum_required (VERSION 3.26)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    "src/codegen/FakeOutputFormat.cpp"
)

set(HEADERS
    "include/codegen/FakeOutputFormat.h"
)

add_library(vasm-fakes-vasm-fakes ${SOURCES} ${HEADERS})
add_library(vasm::fakes ALIAS vasm-fakes-vasm-fakes)
target_include_directories(vasm-fakes-vasm-fakes
    PUBLIC
        include
)
target_compile_features(vasm-fakes-vasm-fakes PUBLIC cxx_std_20)
target_link_libraries(vasm-fakes-vasm-fakes vasm::framework)