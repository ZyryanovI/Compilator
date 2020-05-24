#pragma once

#include "Symbol.hpp"
#include "../AST/Types.h"
#include "VarInfo.hpp"
#include <unordered_map>

class MethodInfo {
public:
    Type* type;
    Symbol* name;
    std::unordered_map<Symbol*, VariableInfo*> args;
    std::unordered_map<Symbol*, VariableInfo*> locals;
    int first_line;
    int first_column;

    MethodInfo(Type *type, Symbol *name, int first_line, int first_column)
        : type(type), name(name), first_line(first_line), first_column(first_column) {}
};
