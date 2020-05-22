#include <iostream>
#include "Goal.h"
#include "../Visitor.h"

Goal::Goal(int first_line, int first_column, IMainClass *main_class, std::vector<std::unique_ptr<IClassDeclaration>> *class_declarations) {
    if (main_class == nullptr || class_declarations == nullptr){
        std::cout<< "There is nullptr in Goal initialization"<<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->main_class = std::unique_ptr<IMainClass> (main_class);
    this->class_declarations = std::unique_ptr<std::vector<std::unique_ptr<IClassDeclaration>>>(class_declarations);
}

void Goal::Accept(Visitor *visitor) const {
    visitor->visit(this);
}

