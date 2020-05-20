#pragma once
#include "Visitor.h"

class Printer : public Visitor
{
private:
    FILE* f;

public:
    Printer(FILE* outFile);

    void visit(const Goal* n) override;
    void visit(const MainClass* n) override;
    void visit(const ClassDeclaration* n) override;
    void visit(const VarDeclaration* n) override;
    void visit(const MethodDeclaration* n) override;

    // void visit(const IntType* n) override;
    // void visit(const BoolType* n) override;
    // void visit(const IntArrayType* n) override;
    void visit(const Type* n) override;

    void visit(const IfStatement* n) override;
    void visit(const WhileStatement* n) override;
    void visit(const Statement* n) override;
    void visit(const PrintStatement* n) override;
    void visit(const AssignmentStatement* n) override;
    void visit(const ArrAssignmentStatement* n) override;

    void visit(const AndExpression* n) override;

    void visit(const LessExpression* n) override;

    void visit(const PlusExpression* n) override;

    void visit(const MinusExpression* n) override;

    void visit(const MultExpression* n) override;

    void visit(const OrExpression* n) override;

    void visit(const RemainExpression* n) override;




    void visit(const ArrayExpression* n) override;

    void visit(const LengthExpression* n) override;

    void visit(const MethodExpression* n) override;

    void visit(const Integer* n) override;

    void visit(const Bool* n) override;

    void visit(const IdentExpression* n) override;

    void visit(const This* n) override;

    void visit(const NewArrExpression* n) override;

    void visit(const NewExpression* n) override;

    void visit(const NotExpression* n) override;

    void visit(const Expression* n) override;

    void visit(const Identifier* n) override;
};
