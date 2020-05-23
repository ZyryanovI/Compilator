#pragma once

#include "AST/Goal.h"
#include "AST/MainClass.h"
#include "AST/ClassDeclaration.h"
#include "AST/VarDeclaration.h"
#include "AST/MethodDeclaration.h"
#include "AST/Identifier.h"
#include "AST/Types.h"
#include "AST/Expressions.h"
#include "AST/Statements.h"


class Visitor{
public:
    virtual void visit(const Goal*) = 0;
    virtual void visit(const MainClass*) = 0;
    virtual void visit(const ClassDeclaration*) = 0;
    virtual void visit(const VarDeclaration*) = 0;
    virtual void visit(const MethodDeclaration*) = 0;

    virtual void visit(const Identifier*) = 0;

    virtual void visit(const Type*) = 0;

    virtual void visit(const MinusExpression*) = 0;
    virtual void visit(const PlusExpression*) = 0;
    virtual void visit(const AndExpression*) = 0;
    virtual void visit(const LessExpression*) = 0;
    virtual void visit(const MultExpression*) = 0;

    virtual void visit(const ArrayExpression*) = 0;
    virtual void visit(const LengthExpression*) = 0;
    virtual void visit(const MethodExpression*) = 0;

    virtual void visit(const Integer*) = 0;
    virtual void visit(const Bool*) = 0;
    virtual void visit(const IdentExpression*) = 0;

    virtual void visit(const This*) = 0;
    virtual void visit(const NewArrExpression*) = 0;
    virtual void visit(const NewExpression*) = 0;
    virtual void visit(const NotExpression*) = 0;
    virtual void visit(const Expression*) = 0;

    virtual void visit(const IfStatement*) = 0;
    virtual void visit(const WhileStatement*) = 0;
    virtual void visit(const Statement*) = 0;
    virtual void visit(const PrintStatement*) = 0;
    virtual void visit(const AssignmentStatement*) = 0;
    virtual void visit(const ArrAssignmentStatement*) = 0;
};

