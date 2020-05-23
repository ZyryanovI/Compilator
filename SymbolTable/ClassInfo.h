#pragma once

#include "Symbol.h"
#include "MethodInfo.h"
#include "VarInfo.h"
#include <unordered_map>

class ClassInfo {
public:
    Symbol* name;
    Symbol* parent;
    ClassInfo* parentInfo;
    std::unordered_map<Symbol*, VariableInfo*> vars;
    std::unordered_map<Symbol*, MethodInfo*> methods;
    int first_line;
    int first_column;

    ClassInfo(Symbol* name, Symbol* parent, int first_line, int first_column);

    bool HasField(Symbol* field_name);
    bool HasMethod(Symbol* method_name);

    MethodInfo* GetMethod(Symbol* method_name);
    int GetSize();
};
