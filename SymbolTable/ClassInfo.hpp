#pragma once

#include "Symbol.hpp"
#include "MethodInfo.hpp"
#include "VarInfo.hpp"
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

    ClassInfo(Symbol *name, Symbol *parent, int first_line, int first_column): name(name),
          parent(parent), parentInfo(nullptr), first_line(first_line), first_column(first_column) {}

    bool HasField(Symbol *field_name) {
        if (vars.find(field_name) != vars.end()) {
            return true;
        }
        if (parentInfo != nullptr) {
            return parentInfo->HasField(field_name);
        }
        return false;
    }

    bool HasMethod(Symbol *method_name) {
        if (methods.find(method_name) != methods.end()) {
            return true;
        }
        if (parentInfo != nullptr) {
            return parentInfo->HasMethod(method_name);
        }
        return false;
    }

    MethodInfo* GetMethod(Symbol *method_name) {
        if (methods.find(method_name) != methods.end()) {
            return methods[method_name];
        }
        if (parentInfo != nullptr) {
            return parentInfo->GetMethod(method_name);
        }
        return nullptr;
    }

    int GetSize() {
        if (parentInfo == nullptr){
            return vars.size();
        }
        return vars.size() + parentInfo->GetSize();
    }
};
