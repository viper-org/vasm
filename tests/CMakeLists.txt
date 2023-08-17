cmake_minimum_required (VERSION 3.26)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    "src/main.cpp"
    "src/Test.cpp"

    "src/lexer/LexerTests.cpp"

    "src/parser/ParserTests.cpp"
)

set(HEADERS
    "include/Assert.h"
    "include/Test.h"
)

add_executable(vasm-tests-vasm-tests ${SOURCES} ${HEADERS})
add_executable(vasm::tests ALIAS vasm-tests-vasm-tests)
target_include_directories(vasm-framework-vasm-framework
    PUBLIC
        include
)
target_compile_features(vasm-tests-vasm-tests PUBLIC cxx_std_20)
target_link_libraries(vasm-tests-vasm-tests vasm::framework)
target_link_libraries(vasm-tests-vasm-tests vasm::fakes)