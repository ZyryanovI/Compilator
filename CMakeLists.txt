cmake_minimum_required(VERSION 3.15)
project(Compilator)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(AST)

add_executable(Compilator
        AST/Goal.cpp
        AST/Goal.h
        AST/Interfaces.h
        main.cpp
        Visitor.h AST/MainClass.h AST/MainClass.cpp AST/ClassDeclaration.h AST/ClassDeclaration.cpp AST/VarDeclaration.h AST/VarDeclaration.cpp AST/MethodDeclaration.h AST/MethodDeclaration.cpp AST/Identifier.h AST/Identifier.cpp AST/Types.h AST/Types.cpp SymbolTable/Symbol.h SymbolTable/Symbol.cpp AST/Statements.h AST/Statements.cpp AST/Expressions.h AST/Expressions.cpp SymbolTable/ClassInfo.h SymbolTable/ClassInfo.cpp SymbolTable/VarInfo.h SymbolTable/VarInfo.cpp SymbolTable/MethodInfo.h SymbolTable/MethodInfo.cpp SymbolTable/Table.h)
