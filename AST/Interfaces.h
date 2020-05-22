#pragma once

class Visitor;

class IAstNode{
public:
    int first_line;
    int first_column;
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
