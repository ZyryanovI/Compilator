#pragma once

#include "Interfaces.hpp"

class MethodDeclaration : public IMethodDeclaration {
public:
    std::unique_ptr<IType> return_type;
    std::unique_ptr<IIdentifier> name;
    std::unique_ptr<std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>> args;
    std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>> vars;
    std::unique_ptr<std::vector<std::unique_ptr<IStatement>>> statements;
    std::unique_ptr<IExpression> return_expression;

    // конструктор
    MethodDeclaration(int first_line, int first_column, IType *return_type, IIdentifier *name,
        std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>> *args, std::vector<std::unique_ptr<IVarDeclaration>> *vars,
        std::vector<std::unique_ptr<IStatement>> *statements, IExpression *return_expression) {

    if (return_type == nullptr || name == nullptr || args == nullptr || vars == nullptr || statements == nullptr || return_expression == nullptr) {
        std::cout<< "There is nullptr in MethodDeclaration initialization"<<std::endl;
    }

    this->first_line = first_line;
    this->first_column = first_column;

    this->return_type = std::unique_ptr<IType>(return_type);
    this->name = std::unique_ptr<IIdentifier>(name);
    this->args = std::unique_ptr<std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>>(args);

    this->vars = std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>>(vars);
    this->statements = std::unique_ptr<std::vector<std::unique_ptr<IStatement>>>(statements);
    this->return_expression = std::unique_ptr<IExpression>(return_expression);
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}
};
