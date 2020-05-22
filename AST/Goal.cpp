#include <iostream>
#include "Goal.h"
#include "../Visitor.h"

Goal::Goal(Position position_new, IMainClass *main_class, std::vector <IClassDeclaration>
           <unique_ptr> *class_declarations) {
    if (main_class == nullptr || class_declarations == nullptr){
        std::cout<< "There is nullptr in Goal initialization"<<std::endl;
    }
    this->position = position_new;
    this->main_class = std::unique<IMainClass> (main_class);
    this->class_declarations = std::unique_ptr<std::vector<std::unique_ptr<IClassDeclaration>>>(class_declarations);
}

void Goal::Accept(Visitor *visitor) const {
    visitor->visit(this);
}

