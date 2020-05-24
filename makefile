# минимальная версия cmake
cmake_minimum_required(VERSION 3.10)

# имя проекта
project(Compilator VERSION 2.0)

all: out

parser.tab.c parser.tab.h: parser.y
	bison -d -v -l parser.y

parser.lexer.c parser.lexer.h: lexer.l parser.tab.c parser.tab.h
	flex lexer.l


# тут обьявляем флаги, которые потом используем при запуске через clang
DEBUG_FLAGS=-fsanitize=address,undefined,leak -g

CXXFLAGS=-std=c++17

# эти файлы нужны для аст
AST=AST/Identifier.hpp AST/Expressions.hpp AST/Statements.cpp AST/Types.hpp AST/VarDeclaration.hpp AST/MethodDeclaration.hpp AST/ClassDeclaration.hpp AST/MainClass.hpp AST/Goal.hpp

# а эти для таблицы символов
SYMBOLTABLE=SymbolTable/Symbol.cpp SymbolTable/ClassInfo.cpp SymbolTable/MethodInfo.cpp SymbolTable/VarInfo.cpp

# куда пишем: что
# команда для сборки
parser.tab.o: parser.tab.c parser.tab.h
	clang++ ${CXXFLAGS} -Wno-deprecated -c parser.tab.c -o parser.tab.o

parser.lexer.o: parser.lexer.c parser.lexer.h
	clang++ ${CXXFLAGS} -Wno-deprecated -c parser.lexer.c -o parser.lexer.o

TableBuilder.o: SymbolTable/TableBuilder.cpp
	clang++ ${CXXFLAGS} -c SymbolTable/TableBuilder.cpp -o TableBuilder.o

ASTPrinter.o: ASTPrinter.cpp
	clang++ ${CXXFLAGS} -c ASTPrinter.cpp -o ASTPrinter.o

out: parser.lexer.o parser.tab.o TableBuilder.o ASTPrinter.o main.cpp ${AST} ${SYMBOLTABLE}
	clang++ ${CXXFLAGS} main.cpp \
		parser.tab.o parser.lexer.o TableBuilder.o ASTPrinter.o \
		 ${AST} ${SYMBOLTABLE} -o out
		 
#чистит проект
clean:
	rm -f out parser.lexer.c parser.lexer.h parser.tab.c parser.tab.h parser.output *.dot *.o *.hh *.cc
