#pragma once

#include "AST/Goal.h"
#include "AST/MainClass.h"

//дополнить для остальных классов, когда напишу их
class Visitor{
public:
    virtual void visit(const Goal*) = 0;
    virtual void visit(const MainClass*) = 0;
    virtual void visit(const ClassDeclaration*) = 0;
    virtual void visit(const VarDeclaration*) = 0;
    virtual void visit(const MethodDeclaration*) = 0;
};

