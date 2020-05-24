all: out

parser.tab.c parser.tab.h: parser.y
	bison -d -v -l parser.y

parser.lexer.c parser.lexer.h: lexer.l parser.tab.c parser.tab.h
	flex lexer.l

DEBUG_FLAGS=-fsanitize=address,undefined,leak -g

CXXFLAGS=-std=c++17

AST=AST/Identifier.cpp AST/Expression.cpp AST/Statement.cpp AST/Type.cpp AST/VarDeclaration.cpp AST/MethodDeclaration.cpp AST/ClassDeclaration.cpp AST/MainClass.cpp AST/Goal.cpp

PRINTER=ASTPrinter.cpp

SYMBOLTABLE=SymbolTable/Symbol.cpp SymbolTable/ClassInfo.cpp SymbolTable/MethodInfo.cpp SymbolTable/VarInfo.cpp

parser.tab.o: parser.tab.c parser.tab.h
	clang++ ${CXXFLAGS} -Wno-deprecated -c parser.tab.c -o parser.tab.o

parser.lexer.o: parser.lexer.c parser.lexer.h
	clang++ ${CXXFLAGS} -Wno-deprecated -c parser.lexer.c -o parser.lexer.o

TableBuilder.o: SymbolTable/TableBuilder.cpp
	clang++ ${CXXFLAGS} -c SymbolTable/TableBuilder.cpp -o TableBuilder.o

ASTPrinter.o: ASTPrinter.cpp
	clang++ ${CXXFLAGS} -c ASTPrinter.cpp -o ASTPrinter.o

out: parser.lexer.o parser.tab.o TableBuilder.o Printer_graph.o main.cpp ${AST} ${PRINTER} ${SYMBOLTABLE}
	clang++ ${CXXFLAGS} main.cpp \
		parser.tab.o parser.lexer.o TableBuilder.o Printer_graph.o \
		 ${AST} ${PRINTER} ${SYMBOLTABLE} -o out

clean:
	rm -f out parser.lexer.c parser.lexer.h parser.tab.c parser.tab.h parser.output *.dot *.o *.hh *.cc
