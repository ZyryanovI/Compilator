#pragma once

#include "Symbol.h"
#include "../AST/Types.h"

class VariableInfo {
public:
    Type* type;
    Symbol* symbol;
    int first_line;
    int first_column;

    VariableInfo() = default;

    VariableInfo(Type* type, Symbol* symbol, int first_line, int first_column);
};
