cmake_minimum_required(VERSION 3.23)
project(hash)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        doctest
        URL https://github.com/onqtam/doctest/archive/refs/tags/2.4.6.zip
)
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
    add_subdirectory(${doctest_SOURCE_DIR} ${doctest_BINARY_DIR})
endif()

add_executable(hash main.cpp hash.h)
target_link_libraries(hash doctest::doctest)
