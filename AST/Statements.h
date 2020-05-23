#pragma once

#include "Interfaces.h"
#include <vector>
#include <memory>

class Statement : public IStatement {
public:
    std::unique_ptr<std::vector<std::unique_ptr<IStatement>>> statements;

    Statement(int first_line, int first_column, std::vector<std::unique_ptr<IStatement>>* statements);

    void Accept(Visitor* visitor) const;
};

class IfStatement : public IStatement {
public:
    std::unique_ptr<IExpression> clause;
    std::unique_ptr<IStatement> true_statement;
    std::unique_ptr<IStatement> false_statement;

    IfStatement(int first_line, int first_column, IExpression* clause, IStatement* true_statement, IStatement* false_statement);

    void Accept(Visitor* visitor) const;
};

class WhileStatement : public IStatement {
public:
    std::unique_ptr<IExpression> clause;
    std::unique_ptr<IStatement> body;

    WhileStatement(int first_line, int first_column, IExpression* clause, IStatement* body);

    void Accept(Visitor* visitor) const;
};

class PrintStatement : public IStatement {
public:
    std::unique_ptr<IExpression> print;

    PrintStatement(int first_line, int first_column, IExpression* print);

    void Accept(Visitor* visitor) const;
};

class AssignmentStatement : public IStatement {
public:
    std::unique_ptr<IIdentifier> var;
    std::unique_ptr<IExpression> expr;

    AssignmentStatement(int first_line, int first_column, IIdentifier* var, IExpression* expr);

    void Accept(Visitor* visitor) const;
};

class ArrAssignmentStatement : public IStatement {
public:
    std::unique_ptr<IIdentifier> var;
    std::unique_ptr<IExpression> num;
    std::unique_ptr<IExpression> expr;

    ArrAssignmentStatement(int first_line, int first_column, IIdentifier* var, IExpression* num, IExpression* expr);

    void Accept(Visitor* v) const;
};
