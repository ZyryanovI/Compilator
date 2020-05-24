#pragma once

/*Класс, отвечающий за вход в AST. Содержит ссылку на класс Main и вектор ссылок на остальные классы*/

#include <vector>
#include <memory>

#include "Interfaces.h"
#include "../Visitor.h"

class Goal: public IGoal{
public:
    std::unique_ptr<IMainClass> main_class; // указатель на main
    std::unique_ptr<std::vector<std::unique_ptr<IClassDeclaration>>> class_declarations; //указатель на список остальных классов

    // конструктор
    Goal(int first_line, int first_column, IMainClass *main_class, std::vector<std::unique_ptr<IClassDeclaration>> *class_declarations) {
    if (main_class == nullptr || class_declarations == nullptr){
        std::cout<< "There is nullptr in Goal initialization"<<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->main_class = std::unique_ptr<IMainClass> (main_class);
    this->class_declarations = std::unique_ptr<std::vector<std::unique_ptr<IClassDeclaration>>>(class_declarations);
}
    // Acccept вызывает метод visit у аргумента типа Visitor (см. Visitor.h)
    void Goal::Accept(Visitor *visitor) const {
    visitor->visit(this);
}

};
