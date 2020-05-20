#pragma once
#include "Symbol.h"
#include "../AST/Type.h"

class VariableInfo {
public:
    VariableInfo() = default;
    VariableInfo(Type* type_, Symbol* symbol_, int first_line, int first_column);

    Type* type;
    Symbol* symbol;
    int first_line;
    int first_column;
};
