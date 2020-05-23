#include <iostream>
#include "Identifier.h"
#include "../Visitor.h"

Identifier::Identifier(int first_line, int first_column, Symbol* name) {
    if (name == nullptr) {
        std::cout << "Empty name in Identifier!" << std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->name = name;
}

void Identifier::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

