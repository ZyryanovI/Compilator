#pragma once
#include "INode.h"
#include <vector>
#include <memory>

class Goal : public IGoal {
public:
    Goal(int first_line, int first_column, IMainClass* main_class,
         std::vector<std::unique_ptr<IClassDeclaration>>* class_declarations);
    void Accept(Visitor* v) const override;

public:
    std::unique_ptr<IMainClass> main_class;
    std::unique_ptr<std::vector<std::unique_ptr<IClassDeclaration>>> class_declarations;
};
