#pragma once

#include "Interfaces.h"
#include <vector>
#include <memory>
#include <utility>


class MethodDeclaration : public IMethodDeclaration {
public:
    std::unique_ptr<IType> return_type;
    std::unique_ptr<IIdentifier> name;
    std::unique_ptr<std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>> args;
    std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>> vars;
    std::unique_ptr<std::vector<std::unique_ptr<IStatement>>> statements;
    std::unique_ptr<IExpression> return_expression;

    MethodDeclaration(int first_line, int first_column, IType* return_type, IIdentifier* name,
            std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>* args,
            std::vector<std::unique_ptr<IVarDeclaration>>* vars,
            std::vector<std::unique_ptr<IStatement>>* statements, IExpression* return_expression);

    void Accept(Visitor* visitor) const;
};