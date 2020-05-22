cmake_minimum_required(VERSION 3.15)
project(Compilator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(AST)

add_executable(Compilator
        AST/Goal.cpp
        AST/Goal.h
        AST/Interfaces.h
        main.cpp
        Visitor.h)
