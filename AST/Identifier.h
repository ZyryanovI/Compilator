#pragma once

#include "Interfaces.h"
#include "../Visitor.h"
#include "../SymbolTable/Symbol.h"

#include <string>


class Identifier : public IIdentifier {
public:
    Symbol* name;

Identifier(int first_line, int first_column, Symbol* name) {
    if (name == nullptr) {
        std::cout << "Empty name in Identifier!" << std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->name = name;
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}

};
