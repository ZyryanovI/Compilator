#pragma once

#include "Symbol.h"
#include "../AST/Types.h"
#include "VarInfo.h"
#include <unordered_map>

class MethodInfo {
public:
    Type* type;
    Symbol* name;
    std::unordered_map<Symbol*, VariableInfo*> args;
    std::unordered_map<Symbol*, VariableInfo*> locals;
    int first_line;
    int first_column;

    MethodInfo(Type* type, Symbol* name, int first_line, int first_column);
};
