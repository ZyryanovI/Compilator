#include "InRegAccess.h"

InRegAccess::InRegAccess(std::string name) : name(name) {
}

IRExpression *InRegAccess::GetExp() {
    return new TempExpression(name);    // 123
}
