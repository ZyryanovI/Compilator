#pragma once
#include "INode.h"
#include <vector>
#include <memory>

class ClassDeclaration : public IClassDeclaration {
public:
    ClassDeclaration(int first_line, int first_column, IIdentifier* class_name,
                     IIdentifier* extends_class_name,
                     std::vector<std::unique_ptr<IVarDeclaration>>* vars,
                     std::vector<std::unique_ptr<IMethodDeclaration>>* methods);
    void Accept(Visitor* v) const override;

public:
    std::unique_ptr<IIdentifier> class_name;
    std::unique_ptr<IIdentifier> extends_class_name;
    std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>> vars;
    std::unique_ptr<std::vector<std::unique_ptr<IMethodDeclaration>>> methods;
};
