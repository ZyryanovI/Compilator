cmake_minimum_required(VERSION 3.10)
project(compilers)

set(CMAKE_CXX_STANDARD 17)

add_executable(compilers main.cpp)

find_package(BISON)
find_package(FLEX)

