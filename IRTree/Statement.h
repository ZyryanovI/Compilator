// #include "../SymbolTable/Symbol.h"
#pragma once

#include "IRStatement.h"
#include "IRExpression.h"
#include "Label.h"
#include <vector>
#include <string>
#include <memory>

class MoveStatement : public IRStatement {   // 3конец, 7+ шаг,2 раза подряд
public:
    MoveStatement(const IRExpression* dst, const IRExpression* src);

    MoveStatement( std::unique_ptr<const IRExpression> dst, std::unique_ptr<const IRExpression> src ) :
    dst( std::move( dst ) ), src( std::move( src ) )
    {}  // mb move to .cpp

    const IRExpression* GetDst() const { return dst.get(); }
    const IRExpression* GetSrc() const { return src.get(); }

    void Accept(IRVisitor* v) const override;

    std::unique_ptr<const IRStatement> GetCopy() const override;

    std::unique_ptr<const IRExpression> dst;
    std::unique_ptr<const IRExpression> src;
};

class ExpStatement : public IRStatement {
public:
    //mb тут explicit
    explicit ExpStatement( std::unique_ptr<const IRExpression> exp ) :
            exp( std::move( exp ) )
    {}  // mb move to .cpp

    explicit ExpStatement(const IRExpression* exp);




    void Accept(IRVisitor* v) const override;
    const IRExpression* GetExp() const { return exp.get(); }

    std::unique_ptr<const IRStatement> GetCopy() const override;

    std::unique_ptr<const IRExpression> exp;
};

class JumpStatement : public IRStatement {
public:
    explicit JumpStatement(const std::string& label_) : label( label_ ) {}
    explicit JumpStatement(const Label& label_) : label( label_ ) {}

    void Accept(IRVisitor* v) const override;

    std::unique_ptr<const IRStatement> GetCopy() const override;

    Label label;
};

class CJumpStatement : public IRStatement {
public:
    enum Relation { LESS, NEQ, EQ };

    CJumpStatement(CJumpStatement::Relation rel, const IRExpression* left, const IRExpression* right,
                   const std::string& if_left, const std::string& if_right);


    CJumpStatement(CJumpStatement::Relation rel, std::unique_ptr<const IRExpression> left,
            std::unique_ptr<const IRExpression> right, const std::string& if_left, const std::string& if_right);

    CJumpStatement(CJumpStatement::Relation rel, const IRExpression* left, const IRExpression* right,
                   const Label& if_left, const Label& if_right);


    CJumpStatement(CJumpStatement::Relation rel, std::unique_ptr<const IRExpression> left,
                   std::unique_ptr<const IRExpression> right, const Label& if_left, const Label& if_right);


    void Accept(IRVisitor* v) const override;

    const IRExpression* GetLeft() const { return left.get(); }
    const IRExpression* GetRight() const { return right.get(); }
    const Label& GetTrueLabel() const { return if_left; }
    const Label& GetFalseLabel() const { return if_right; }
    const Relation GetType() const {return rel; }

    std::unique_ptr<const IRStatement> GetCopy() const override;

    CJumpStatement::Relation rel;
    std::unique_ptr<const IRExpression> left;
    std::unique_ptr<const IRExpression> right;
    Label if_left;
    Label if_right;
};

class SeqStatement : public IRStatement {
public:
    SeqStatement(const IRStatement* left, const IRStatement* right);

    SeqStatement( std::unique_ptr<const IRStatement> left, std::unique_ptr<const IRStatement> right ) :
    left( std::move( left ) ), right( std::move( right ) )
    {
    }

    const IRStatement* GetLeft() const { return left.get(); }
    const IRStatement* GetRight() const { return right.get(); }

    void Accept(IRVisitor* v) const override;

    std::unique_ptr<const IRStatement> GetCopy() const override;

    std::unique_ptr<const IRStatement> left;
    std::unique_ptr<const IRStatement> right;
};

class LabelStatement : public IRStatement {
public:
//    LabelStatement(std::string& label);
    explicit LabelStatement(const std::string& label) : label( label ) {}
    explicit LabelStatement(const Label& label) : label( label ) {}

    void Accept(IRVisitor* v) const override;

    const Label& GetLabel() const;


    std::unique_ptr<const IRStatement> GetCopy() const override;
    Label label;
};
