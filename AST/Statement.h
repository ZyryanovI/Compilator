#ifndef AST_Statement
#define AST_Statement

#include "INode.h"
#include <vector>
#include <memory>

class Statement : public IStatement {
public:
    Statement(int first_line, int first_column,
              std::vector<std::unique_ptr<IStatement>>* statements);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<std::vector<std::unique_ptr<IStatement>>> statements;
};

class IfStatement : public IStatement {
public:
    IfStatement(int first_line, int first_column, IExpression* clause, IStatement* true_statement,
                IStatement* false_statement);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> clause;
    std::unique_ptr<IStatement> true_statement;
    std::unique_ptr<IStatement> false_statement;
};

class WhileStatement : public IStatement {
public:
    WhileStatement(int first_line, int first_column, IExpression* clause, IStatement* body);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> clause;
    std::unique_ptr<IStatement> body;
};

class PrintStatement : public IStatement {
public:
    PrintStatement(int first_line, int first_column, IExpression* print);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> print;
};

class AssignmentStatement : public IStatement {
public:
    AssignmentStatement(int first_line, int first_column, IIdentifier* var, IExpression* expr);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IIdentifier> var;
    std::unique_ptr<IExpression> expr;
};

class ArrAssignmentStatement : public IStatement {
public:
    ArrAssignmentStatement(int first_line, int first_column, IIdentifier* var, IExpression* num,
                           IExpression* expr);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IIdentifier> var;
    std::unique_ptr<IExpression> num;
    std::unique_ptr<IExpression> expr;
};

#endif  // AST_Statement
