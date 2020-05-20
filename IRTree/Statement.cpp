#include "Statement.h"
#include "IRVisitor.h"
#include <string.h>
#include <iostream>

const Label& LabelStatement::GetLabel() const
{
    return label;
}


// 1
std::unique_ptr<const IRStatement> ExpStatement::GetCopy() const
{
    return std::move(std::make_unique<const ExpStatement>( exp->GetCopy() ) );
}

std::unique_ptr<const IRStatement> JumpStatement::GetCopy() const
{
    return std::move(std::make_unique<const JumpStatement>( label ) );
}

std::unique_ptr<const IRStatement> CJumpStatement::GetCopy() const
{
    return std::move(std::make_unique<const CJumpStatement>( rel, left->GetCopy(), right->GetCopy(),
                                                   if_left, if_right ) );
}

std::unique_ptr<const IRStatement> SeqStatement::GetCopy() const
{
    return std::move(std::make_unique<const SeqStatement>( left->GetCopy(), right->GetCopy() ) );
}

std::unique_ptr<const IRStatement> LabelStatement::GetCopy() const
{
    return std::move(std::make_unique<LabelStatement>( label ) );
}

// 2
std::unique_ptr<const IRStatement> MoveStatement::GetCopy() const
{
    //return std::move(std::make_unique<const MoveStatement>( dst->GetCopy().get(), src->GetCopy().get() ) );
    return std::move(std::make_unique<const MoveStatement>( dst->GetCopy(), src->GetCopy() ) );
}





//end




MoveStatement::MoveStatement(const IRExpression *dst, const IRExpression *src) : dst(dst), src(src) {
}

void MoveStatement::Accept(IRVisitor *v) const {
    v->visit(this);
}

ExpStatement::ExpStatement(const IRExpression *exp) : exp(exp) {
}

void ExpStatement::Accept(IRVisitor *v) const {
    v->visit(this);
}

//JumpStatement::JumpStatement(std::string &label) : label(label) {
//}

void JumpStatement::Accept(IRVisitor *v) const {
    v->visit(this);
}

CJumpStatement::CJumpStatement(CJumpStatement::Relation rel, const IRExpression *left,
                               const IRExpression *right, const std::string &if_left, const std::string &if_right)
    : rel(rel), left(left), right(right), if_left(if_left), if_right(if_right) {
}

CJumpStatement::CJumpStatement(CJumpStatement::Relation rel, std::unique_ptr<const IRExpression> left,
                               std::unique_ptr<const IRExpression> right,
                               const std::string& if_left,
                               const std::string& if_right) :
        rel( rel ),
        left( std::move( left ) ),
        right( std::move( right ) ),
        if_left( if_left ),
        if_right( if_right )
        {
        }


CJumpStatement::CJumpStatement(CJumpStatement::Relation rel, const IRExpression *left,
                               const IRExpression *right, const Label& if_left, const Label& if_right)
        : rel(rel), left(left), right(right), if_left(if_left), if_right(if_right) {
}

CJumpStatement::CJumpStatement(CJumpStatement::Relation rel, std::unique_ptr<const IRExpression> left,
                               std::unique_ptr<const IRExpression> right,
                               const Label& if_left,
                               const Label& if_right) :
        rel( rel ),
        left( std::move( left ) ),
        right( std::move( right ) ),
        if_left( if_left ),
        if_right( if_right )
{
}




void CJumpStatement::Accept(IRVisitor *v) const {
    v->visit(this);
}

SeqStatement::SeqStatement(const IRStatement *left, const IRStatement *right) : left(left), right(right) {
}

void SeqStatement::Accept(IRVisitor *v) const {
    v->visit(this);
}

//LabelStatement::LabelStatement(std::string &label) : label(label) {
//}

void LabelStatement::Accept(IRVisitor *v) const {
    v->visit(this);
}
