#include "Symbol.h"
#include "Table.h"
#include "ClassInfo.h"
#include "MethodInfo.h"
#include "VarInfo.h"
#include "../Visitor.h"

#include <vector>
#include <string>

class TableBuilder : Visitor {
private:
    Table* table;
    ClassInfo* curClass;
    MethodInfo* curMethod;
    VariableInfo* curVar;
    Type curType;
    bool isvalidExpr;
    std::vector<std::string> errors;

public:

    TableBuilder();

    Table* buildTable(Goal* goal) {
        visit(goal);
        printErrors();
        return table;
    }

    void printErrors();

    bool hasClass(Symbol* className);

    virtual void visit(const Goal*) override ;
    virtual void visit(const MainClass*) override;
    virtual void visit(const ClassDeclaration*) override;
    virtual void visit(const VarDeclaration*) override;
    virtual void visit(const MethodDeclaration*) override;

    virtual void visit(const Identifier*) override;

    virtual void visit(const Type*) override;

    virtual void visit(const MinusExpression*) override;
    virtual void visit(const PlusExpression*) override;
    virtual void visit(const AndExpression*) override;
    virtual void visit(const LessExpression*) override;
    virtual void visit(const MultExpression*) override;

    virtual void visit(const ArrayExpression*) override;
    virtual void visit(const LengthExpression*) override;
    virtual void visit(const MethodExpression*) override;

    virtual void visit(const Integer*) override;
    virtual void visit(const Bool*) override;
    virtual void visit(const IdentExpression*) override;

    virtual void visit(const This*) override;
    virtual void visit(const NewArrExpression*) override;
    virtual void visit(const NewExpression*) override;
    virtual void visit(const NotExpression*) override;
    virtual void visit(const Expression*) override;

    virtual void visit(const IfStatement*) override;
    virtual void visit(const WhileStatement*) override;
    virtual void visit(const Statement*) override;
    virtual void visit(const PrintStatement*) override;
    virtual void visit(const AssignmentStatement*) override;
    virtual void visit(const ArrAssignmentStatement*) override;
};
