#include <iostream>
#include "TableBuilder.h"
#include "Symbol.h"
#include <cassert>
#include <exception>

TableBuilder::TableBuilder(): table(new Table), curClass(nullptr), curMethod(nullptr),
          curVar(nullptr), curType(TYPE(NoneType{})), isvalidExpr(true) {}

void TableBuilder::printErrors() {
    for (auto &err : errors) {
        std::cout << err;
    }
    if (!errors.empty()) {
        throw std::exception();
    }
}

bool TableBuilder::hasClass(Symbol *className) {
    return table->classes.find(className) != table->classes.end();
}

//надо сделать
void TableBuilder::visit(const Goal *goal) {
    for (auto &classDecl : *(goal->class_declarations)) {
        classDecl->Accept(this);
    }
    goal->main_class->Accept(this);
}


void TableBuilder::visit(const MainClass *n) {
    
}

