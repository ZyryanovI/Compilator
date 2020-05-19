#include "SubTreeWrapper.h"
#include <cassert>

ExpWrapper::ExpWrapper(IRExpression *exp) : exp(exp) {
}

IRExpression *ExpWrapper::ToExp() {
    return exp;
}

IRStatement *ExpWrapper::ToStm() {
    return new ExpStatement(exp);
}

IRStatement *ExpWrapper::ToCond(std::string &t, std::string &f) {
    return new CJumpStatement(CJumpStatement::Relation::LESS, exp, new ConstExpression(0), t, f);
}

StmWrapper::StmWrapper(IRStatement *stm) : stm(stm) {
}

IRExpression *StmWrapper::ToExp() {
    assert(false);
}

IRStatement *StmWrapper::ToStm() {
    return stm;
}

IRStatement *StmWrapper::ToCond(std::string &, std::string &) {
    assert(false);
}

CondWrapper::CondWrapper(CJumpStatement::Relation rel, IRExpression *left, IRExpression *right)
    : rel(rel), left(left), right(right) {
}

IRExpression *CondWrapper::ToExp() {
    assert(false);
}

IRStatement *CondWrapper::ToStm() {
    assert(false);
}

IRStatement *CondWrapper::ToCond(std::string &t, std::string &f) {
    return new CJumpStatement(rel, left, right, t, f);
}

CondNotWrapper::CondNotWrapper(ISubTreeWrapper *wrapper) : wrapper(wrapper) {
}

IRExpression *CondNotWrapper::ToExp() {
    assert(false);
}

IRStatement *CondNotWrapper::ToStm() {
    assert(false);
}

IRStatement *CondNotWrapper::ToCond(std::string &t, std::string &f) {
    return wrapper->ToCond(f, t);
}
