#include "IRStatement.h"
#include "IRExpression.h"
#include "Statement.h"
#include "Expression.h"
#include <string>
// #include "../SymbolTable/Symbol.h"

class ISubTreeWrapper {
public:
    virtual ~ISubTreeWrapper() = default;

    virtual IRExpression* ToExp() = 0;
    virtual IRStatement* ToStm() = 0;
    virtual IRStatement* ToCond(std::string& t, std::string& f) = 0;
};

class ExpWrapper : public ISubTreeWrapper {
public:
    ExpWrapper(IRExpression* exp);

    IRExpression* ToExp() override;
    IRStatement* ToStm() override;
    IRStatement* ToCond(std::string& t, std::string& f) override;

private:
    IRExpression* exp;
};

class StmWrapper : public ISubTreeWrapper {
public:
    StmWrapper(IRStatement* stm);

    IRExpression* ToExp() override;
    IRStatement* ToStm() override;
    IRStatement* ToCond(std::string& t, std::string& f) override;

private:
    IRStatement* stm;
};

class CondWrapper : public ISubTreeWrapper {
public:
    CondWrapper(CJumpStatement::Relation rel, IRExpression* left, IRExpression* right);

    IRExpression* ToExp() override;
    IRStatement* ToStm() override;
    IRStatement* ToCond(std::string& t, std::string& f) override;

private:
    CJumpStatement::Relation rel;
    IRExpression* left;
    IRExpression* right;
};

class CondNotWrapper : public ISubTreeWrapper {
public:
    CondNotWrapper(ISubTreeWrapper* wrapper);

    IRExpression* ToExp() override;
    IRStatement* ToStm() override;
    IRStatement* ToCond(std::string& t, std::string& f) override;

private:
    ISubTreeWrapper* wrapper;
};
