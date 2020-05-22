#pragma once

#include <vector>
#include "Interfaces.h"
#include <memory>

class Goal: public IGoal{
public:
    std::unique_ptr<IMainClass> main_class; // указатель на main
    std::unique_ptr<std::vector<std::unique_ptr<IClassDeclaration>>> class_declarations; //указатель на список остальных классов

    Goal(int first_line, int first_column, IMainClass* main_class, std::vector<std::unique_ptr<IClassDeclaration>>* class_declarations);

    void Accept(Visitor* visitor) const override;
};
