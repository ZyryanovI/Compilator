#pragma once
#include "INode.h"
#include "../SymbolTable/Symbol.h"
#include <string>

class Identifier : public IIdentifier {
public:
    Identifier(int first_line, int first_column, Symbol* name);
    void Accept(Visitor* v) const;

public:
    Symbol* name;
};
