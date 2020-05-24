#pragma once

// используемые библиотеки

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include "../Visitor.h"

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
class IStatement : public IAstNode {}; //if, while and so on
class IExpression : public IAstNode {};
class IIdentifier : public IAstNode {};
