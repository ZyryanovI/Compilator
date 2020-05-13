#pragma once
#include "INode.h"
#include <vector>
#include <utility>
#include <memory>

class MethodDeclaration : public IMethodDeclaration {
public:
    MethodDeclaration(
        int first_line, int first_column, IType* return_type, IIdentifier* name,
        std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>* args,
        std::vector<std::unique_ptr<IVarDeclaration>>* vars,
        std::vector<std::unique_ptr<IStatement>>* statements, IExpression* return_expression);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IType> return_type;
    std::unique_ptr<IIdentifier> name;
    std::unique_ptr<std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>>
        args;
    // ptr - vector - pair(type, id)
    std::unique_ptr<std::vector<std::unique_ptr<IVarDeclaration>>> vars;
    std::unique_ptr<std::vector<std::unique_ptr<IStatement>>> statements;
    std::unique_ptr<IExpression> return_expression;
};
