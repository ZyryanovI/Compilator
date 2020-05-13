#include "ClassInfo.h"

ClassInfo::ClassInfo(Symbol *name, Symbol *parent, int first_line, int first_column)
    : name(name),
      parent(parent),
      parentInfo(nullptr),
      first_line(first_line),
      first_column(first_column) {
}

bool ClassInfo::HasField(Symbol *field_name) {
    if (vars.find(field_name) != vars.end()) {
        return true;
    }
    if (parentInfo != nullptr) {
        return parentInfo->HasField(field_name);
    }
    return false;
}

bool ClassInfo::HasMethod(Symbol *method_name) {
    if (methods.find(method_name) != methods.end()) {
        return true;
    }
    if (parentInfo != nullptr) {
        return parentInfo->HasMethod(method_name);
    }
    return false;
}

MethodInfo *ClassInfo::GetMethod(Symbol *method_name) {
    if (methods.find(method_name) != methods.end()) {
        return methods[method_name];
    }
    if (parentInfo != nullptr) {
        return parentInfo->GetMethod(method_name);
    }
    return nullptr;
}

int ClassInfo::GetSize() {
    return vars.size() + (parentInfo == nullptr ? 0 : parentInfo->GetSize());
}
