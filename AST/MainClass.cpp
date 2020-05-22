#include <iostream>
#include "MainClass.h"
#include "../Visitor.h"

MainClass::MainClass(int first_line, int first_column, IIdentifier *class_name, IIdentifier *arg, IStatement *statement) {
    if (class_name == nullptr || arg == nullptr || statement == nullptr) {
        std::cout<< "There is nullptr in MainClass initialization"<<std::endl;
    }

    this->first_line = first_line;
    this->first_column = first_column;
    this->class_name = std::unique_ptr<IIdentifier>(class_name);
    this->arg = std::unique_ptr<IIdentifier>(arg);
    this->statement = std::unique_ptr<IStatement>(statement);
}

void MainClass::Accept(Visitor* visitor) const {
    visitor->visit(this);
}
