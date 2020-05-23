#include <iostream>
#include "Identifier.h"

Identifier::Identifier(int first_line, int first_column, std::string name) {
    if (name.empty()) {
        std::cout << "Empty name in Identifier!" << std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->name = name;
}

void Identifier::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

