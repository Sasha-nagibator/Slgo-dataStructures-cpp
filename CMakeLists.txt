cmake_minimum_required(VERSION 3.23)
project(Algo_DataStructures)

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR}/hash)
include_directories(${CMAKE_SOURCE_DIR}/AVLtree)
include_directories(${CMAKE_SOURCE_DIR}/sort)

add_executable(Algo_DataStructures main.cpp)
