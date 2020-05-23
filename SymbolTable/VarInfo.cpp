#include "VarInfo.h"

VariableInfo::VariableInfo(Type *type, Symbol *symbol, int first_line, int first_column)
        : type(type), symbol(symbol), first_line(first_line), first_column(first_column) {}

