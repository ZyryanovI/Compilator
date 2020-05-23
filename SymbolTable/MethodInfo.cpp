#include "MethodInfo.h"

MethodInfo::MethodInfo(Type *type, Symbol *name, int first_line, int first_column)
        : type(type), name(name), first_line(first_line), first_column(first_column) {}

