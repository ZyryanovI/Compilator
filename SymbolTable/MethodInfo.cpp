#include "MethodInfo.h"

MethodInfo::MethodInfo(Type *type_, Symbol *name_, int first_line, int first_column)
    : type(type_), name(name_), first_line(first_line), first_column(first_column) {
}
