#include "Identifier.h"
#include "../Visitor.h"
#include <iostream>

Identifier::Identifier(int first_line, int first_column, Symbol *name) {
    if (name == nullptr) {
        std::cout << "Empty name in Identifier!\n";
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->name = name;
}

void Identifier::Accept(Visitor *v) const {
    v->visit(this);
}
