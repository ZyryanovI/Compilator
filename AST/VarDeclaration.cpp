#include <iostream>
#include "VarDeclaration.h"
#include "../Visitor.h"

VarDeclaration::VarDeclaration(int first_line, int first_column, IType *type, IIdentifier *name) {
    if (type == nullptr || name == nullptr) {
        std::cout<< "There is nullptr in VarDeclaration initialization"<<std::endl;
    }

    this->first_line = first_line;
    this->first_column = first_column;
    this->type = std::unique_ptr<IType>(type);
    this->name = std::unique_ptr<IIdentifier>(name);
}

void VarDeclaration::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

