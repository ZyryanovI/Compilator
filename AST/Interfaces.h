#pragma once

class Visitor;

struct Position{
    Position(int first_line_new, int first_column_new): first_line(first_line_new), first_column(first_column_new) {}
    int first_line;
    int first_column;
};

class IAstNode{
public:
    Position position;
    virtual ~IAstNode() = default;
    virtual void Accept(Visitor* visitor) const = 0;
};

class IGoal : public IAstNode {};
class IMainClass : public IAstNode {};

class IVarDeclaration : public IAstNode {};
class IClassDeclaration : public IAstNode {};
class IMethodDeclaration : public IAstNode {};

class IType : public IAstNode {};
class IStatement : public IAstNode {};
class IExpression : public IAstNode {};
class IIdentifier : public IAstNode {};
