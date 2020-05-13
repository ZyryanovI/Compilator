all: parser

parser.tab.c parser.tab.h: parser.y
	bison -d -v -l parser.y

parser.lexer.c parser.lexer.h: parser.l parser.tab.c parser.tab.h
	flex parser.l

DEBUG_FLAGS=-fsanitize=address,undefined,leak -g

WARNINGS = -Reverything -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic \
		   -Wno-global-constructors -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-prototypes \
		   -Wno-missing-variable-declarations -Wno-shadow-field-in-constructor \
		   -Wno-weak-vtables -Wno-return-std-move-in-c++11 \
		   \
		   -Wno-padded -Wno-exit-time-destructors -Wno-deprecated -Wno-sign-conversion \
		   -Wno-shorten-64-to-32

CXXFLAGS=-std=c++17 #${DEBUG_FLAGS}

AST=AST/Identifier.cpp AST/Expression.cpp AST/Statement.cpp AST/Type.cpp AST/VarDeclaration.cpp AST/MethodDeclaration.cpp AST/ClassDeclaration.cpp AST/MainClass.cpp AST/Goal.cpp

PRINTER=#Printer.cpp

SYMBOLTABLE=SymbolTable/Symbol.cpp SymbolTable/ClassInfo.cpp SymbolTable/MethodInfo.cpp SymbolTable/VariableInfo.cpp

ACTIVATIONRECORDS=ActivationRecords/InFrameAccess.cpp ActivationRecords/InRegAccess.cpp ActivationRecords/X86MiniJavaFrame.cpp

IRTREE=IRTree/Expression.cpp IRTree/Statement.cpp IRTree/SubTreeWrapper.cpp IRTree/Translator.cpp IRTree/IRPrinter.cpp IRTree/canonizers/CallCanon.cpp IRTree/canonizers/ESeqCanon.cpp IRTree/Label.cpp

parser.tab.o: parser.tab.c parser.tab.h
	clang++ ${CXXFLAGS} -Wno-deprecated -c parser.tab.c -o parser.tab.o

parser.lexer.o: parser.lexer.c parser.lexer.h
	clang++ ${CXXFLAGS} -Wno-deprecated -c parser.lexer.c -o parser.lexer.o

TableBuilder.o: SymbolTable/TableBuilder.cpp
	clang++ ${CXXFLAGS} ${WARNINGS} -c SymbolTable/TableBuilder.cpp -o TableBuilder.o

Printer_graph.o: Printer_graph.cpp
	clang++ ${CXXFLAGS} ${WARNINGS} -c Printer_graph.cpp -o Printer_graph.o

parser: parser.lexer.o parser.tab.o TableBuilder.o Printer_graph.o main.cpp ${AST} ${PRINTER} ${SYMBOLTABLE} ${ACTIVATIONRECORDS} ${IRTREE}
	clang++ ${CXXFLAGS} ${WARNINGS} main.cpp \
		parser.tab.o parser.lexer.o TableBuilder.o Printer_graph.o \
		 ${AST} ${PRINTER} ${SYMBOLTABLE} ${ACTIVATIONRECORDS} ${IRTREE} -o parser

clean:
	rm -f parser parser.lexer.c parser.lexer.h parser.tab.c parser.tab.h parser.output *.dot *.o

static:
	scan-build -o /tmp/compilers make
