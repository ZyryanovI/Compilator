#pragma once

#include "Expression.h"
#include "Statement.h"

struct IRVisitor {
    virtual void visit(const ConstExpression* n) = 0;
    virtual void visit(const NameExpression* n) = 0;
    virtual void visit(const TempExpression* n) = 0;
    virtual void visit(const BinOpExpression* n) = 0;
    virtual void visit(const MemExpression* n) = 0;
    virtual void visit(const CallExpression* n) = 0;
    virtual void visit(const ESeqExpression* n) = 0;

    virtual void visit(const MoveStatement* n) = 0;
    virtual void visit(const ExpStatement* n) = 0;
    virtual void visit(const JumpStatement* n) = 0;
    virtual void visit(const CJumpStatement* n) = 0;
    virtual void visit(const SeqStatement* n) = 0;
    virtual void visit(const LabelStatement* n) = 0;

    virtual void visit( const IRExpList* n) = 0;

    virtual ~IRVisitor() = default;
};

// #endif  // IRTree_IRVisitor
