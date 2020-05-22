#pragma once

#include "Interfaces.h"
#include <memory>

class VarDeclaration : public IVarDeclaration {
public:
    std::unique_ptr<IType> type;
    std::unique_ptr<IIdentifier> name;

    VarDeclaration(int first_line, int first_column, IType* type, IIdentifier* name);

    void Accept(Visitor* visitor) const;
};