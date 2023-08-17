cmake_minimum_required (VERSION 3.26)

set(SOURCES
    "src/main.cpp"
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(vasm ${SOURCES} ${HEADERS})
add_executable(vasm::compiler ALIAS vasm)
target_include_directories(vasm
    PUBLIC
        include
)
target_compile_features(vasm PUBLIC cxx_std_20)
target_link_libraries(vasm vasm::framework)