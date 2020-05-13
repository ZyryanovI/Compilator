#pragma once

#include "AST/ClassDeclaration.h"
#include "AST/INode.h"
#include "AST/Goal.h"
#include "AST/MainClass.h"
#include "AST/Identifier.h"
#include "AST/MethodDeclaration.h"
#include "AST/Statement.h"
#include "AST/Expression.h"
#include "AST/Type.h"
#include "AST/VarDeclaration.h"

struct Visitor {
    virtual void visit(const Goal* n) = 0;
    virtual void visit(const MainClass* n) = 0;
    virtual void visit(const ClassDeclaration* n) = 0;
    virtual void visit(const VarDeclaration* n) = 0;
    virtual void visit(const MethodDeclaration* n) = 0;

    virtual void visit(const Type* n) = 0;

    virtual void visit(const IfStatement* n) = 0;
    virtual void visit(const WhileStatement* n) = 0;
    virtual void visit(const Statement* n) = 0;
    virtual void visit(const PrintStatement* n) = 0;
    virtual void visit(const AssignmentStatement* n) = 0;
    virtual void visit(const ArrAssignmentStatement* n) = 0;

    virtual void visit(const MinusExpression* n) = 0;
    virtual void visit(const PlusExpression* n) = 0;
    virtual void visit(const AndExpression* n) = 0;
    virtual void visit(const LessExpression* n) = 0;
    virtual void visit(const MultExpression* n) = 0;

    virtual void visit(const RemainExpression* n) = 0;
    virtual void visit(const OrExpression* n) = 0;
    virtual void visit(const ArrayExpression* n) = 0;
    virtual void visit(const LengthExpression* n) = 0;
    virtual void visit(const MethodExpression* n) = 0;
    virtual void visit(const Integer* n) = 0;
    virtual void visit(const Bool* n) = 0;
    virtual void visit(const IdentExpression* n) = 0;
    virtual void visit(const This* n) = 0;
    virtual void visit(const NewArrExpression* n) = 0;
    virtual void visit(const NewExpression* n) = 0;
    virtual void visit(const NotExpression* n) = 0;
    virtual void visit(const Expression* n) = 0;

    virtual void visit(const Identifier* n) = 0;

    virtual ~Visitor() = default;
};
