#include <iostream>
#include <cstdio>
#include "parser.tab.h"
#include "AST/Goal.h"
#include "ASTPrinter.h"
#include "SymbolTable/Symbol.h"
#include "SymbolTable/Table.h"
#include "SymbolTable/TableBuilder.h"

extern int yyparse(Goal* goal);

extern void yyerror(Goal* goal, const char* msg);

extern FILE *yyin;

int main(int argc, char** argv) {

    FILE* cur_file;

    if (argc == 1){
        std::cout<<"You need to specify input file. Try again" << std::endl;
        return 0;
    }
	cur_file = fopen(argv[1], "r");

    yyin = cur_file;
    Goal* goal = nullptr;
    Table* table = nullptr;

    try {
        //AST
        yyparse(&goal);
        ASTPrinter astPrinter("output.dot");
        astPrinter.visit(goal);

        //SymbolTable
        TableBuilder tableBuilder;
        tableBuilder.buildTable(goal);

    } catch(...){
        fclose(cur_file);
        delete goal;
        delete table;
        return 0;
    }

    fclose(cur_file);
    delete goal;
    delete table;

    std::cout << "successfully ended" <<std::endl;


    return 0;
}
