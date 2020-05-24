#pragma once

#include <vector>
#include <memory>

#include "Interfaces.h"
#include "../Visitor.h"

class ClassDeclaration : public IClassDeclaration {
public:
    std::unique_ptr<IIdentifier> class_name;
    std::unique_ptr<IIdentifier> extends_class_name;
    std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>> vars;
    std::unique_ptr<std::vector<std::unique_ptr<IMethodDeclaration>>> methods;

   ClassDeclaration(int first_line, int first_column, IIdentifier *class_name,
                                   IIdentifier *extends_class_name,
                                   std::vector<std::unique_ptr<IVarDeclaration>> *vars,
                                   std::vector<std::unique_ptr<IMethodDeclaration>> *methods) {
    if (class_name == nullptr || vars == nullptr || methods == nullptr) {
        std::cout<< "There is nullptr in ClassDeclaration initialization"<<std::endl;
    }


    void Accept(Visitor* visitor) const override;
};
