#include "ClassDeclaration.h"
#include "../Visitor.h"
#include <iostream>

ClassDeclaration::ClassDeclaration(int first_line, int first_column, IIdentifier *class_name,
                                   IIdentifier *extends_class_name,
                                   std::vector<std::unique_ptr<IVarDeclaration>> *vars,
                                   std::vector<std::unique_ptr<IMethodDeclaration>> *methods) {
    if (class_name == nullptr || vars == nullptr || methods == nullptr) {
        std::cout << "Nullptr encountered in ClassDeclaration initializer.\n";
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->class_name = std::unique_ptr<IIdentifier>(class_name);
    this->extends_class_name = std::unique_ptr<IIdentifier>(extends_class_name);
    this->vars = std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>>(vars);
    this->methods = std::unique_ptr<std::vector<std::unique_ptr<IMethodDeclaration>>>(methods);
}

void ClassDeclaration::Accept(Visitor *v) const {
    v->visit(this);
}
