cmake_minimum_required(VERSION 3.23)
project(sort)

set(CMAKE_CXX_STANDARD 20)

add_executable(sort main.cpp sort.h tests.cpp)
