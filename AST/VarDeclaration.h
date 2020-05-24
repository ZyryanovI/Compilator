#pragma once

#include "Interfaces.hpp"

class VarDeclaration : public IVarDeclaration {
public:
    std::unique_ptr<IType> type;
    std::unique_ptr<IIdentifier> name;

    VarDeclaration(int first_line, int first_column, IType *type, IIdentifier *name) {
    if (type == nullptr || name == nullptr) {
        std::cout<< "There is nullptr in VarDeclaration initialization"<<std::endl;
    }

    this->first_line = first_line;
    this->first_column = first_column;
    this->type = std::unique_ptr<IType>(type);
    this->name = std::unique_ptr<IIdentifier>(name);
    }

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};
