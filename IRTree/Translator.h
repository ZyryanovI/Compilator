#pragma once
#include "../Visitor.h"
#include "../SymbolTable/Table.h"
#include "../SymbolTable/Symbol.h"
#include "IRStatement.h"
#include "IRExpression.h"
#include "Statement.h"
#include "Expression.h"
#include "CodeFragment.h"
#include "SubTreeWrapper.h"
#include <map>

class Translator : public Visitor {

public:
    Translator(Table* table);
    void AcceptStms(const std::vector<std::unique_ptr<IStatement>>* statements);

    void visit(const Goal* n) override;
    void visit(const MainClass* n) override;
    void visit(const ClassDeclaration* n) override;
    void visit(const VarDeclaration* n) override;
    void visit(const MethodDeclaration* n) override;

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

private:
    std::unique_ptr<ISubTreeWrapper> curWrapper = nullptr;
    Table* table;
    Symbol* callerClass;
    int ifCounter = 0;
    int whileCounter = 0;
};
