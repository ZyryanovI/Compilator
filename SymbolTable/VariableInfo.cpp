#include "VariableInfo.h"

VariableInfo::VariableInfo(Type *type_, Symbol *symbol_, int first_line, int first_column)
    : type(type_), symbol(symbol_), first_line(first_line), first_column(first_column) {
}
