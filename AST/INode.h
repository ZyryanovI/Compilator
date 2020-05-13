#pragma once

struct Visitor;

class INode {
public:
    virtual void Accept(Visitor* v) const = 0;
    virtual ~INode() = default;
    int first_line;
    int first_column;
};

class IGoal : public INode {};

class IMainClass : public INode {};

class IClassDeclaration : public INode {};

class IVarDeclaration : public INode {};

class IMethodDeclaration : public INode {};

class IType : public INode {};

class IStatement : public INode {};

class IExpression : public INode {};

class IIdentifier : public INode {};
