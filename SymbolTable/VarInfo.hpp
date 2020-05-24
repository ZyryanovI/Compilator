#pragma once

#include "Symbol.hpp"
#include "../AST/Types.hpp"

class VariableInfo {
public:
    Type* type;
    Symbol* symbol;
    int first_line;
    int first_column;

    VariableInfo() = default;

    VariableInfo(Type *type, Symbol *symbol, int first_line, int first_column)
        : type(type), symbol(symbol), first_line(first_line), first_column(first_column) {}
};
