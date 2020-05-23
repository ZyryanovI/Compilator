#pragma once
#include "Interfaces.h"
#include <string>
#include "../SymbolTable/Symbol.h"

class Identifier : public IIdentifier {
public:
    Symbol* name;

    Identifier(int first_line, int first_column, Symbol* name);

    void Accept(Visitor* visitor) const;

};