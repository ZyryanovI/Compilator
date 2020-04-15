#pragma once

#include <string>
#include <memory>
#include <iostream>

#include <vector>
#include <yyltype.hpp>
#include <handlers/visitable.hpp>
#include <visitor.hpp>

namespace ast {

/***************************************************************************************************/

class Expression : public IVisitable {
public:
    Expression() = default;
protected:
    void setPos(const MiniJava::YYLTYPE pos) { pos_ = pos; }
};

typedef std::shared_ptr<Expression> PExpression;

/***************************************************************************************************/

class ExpressionInt : public Expression {
public:
    ExpressionInt(int &value, MiniJava::YYLTYPE pos) : value_(value) { setPos(pos); }

    int getValue() const { return value_; }

    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    int value_;
};

typedef std::shared_ptr<ExpressionInt> PExpressionInt;


class ExpressionBinaryOp: public Expression {
public:
    ExpressionBinaryOp(
        PExpression &left,
        PExpression &right,
        std::string &operation,
        const MiniJava::YYLTYPE pos
    ) :
        left_(left), right_(right), operation_(operation) {
            setPos(pos);
        }

    const PExpression & getLeft() const { return left_; };
    const PExpression & getRight() const { return right_; };
    const std::string & getOp() const { return operation_; };
    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    std::string operation_;
    PExpression left_;
    PExpression right_;
};

typedef std::shared_ptr<ExpressionBinaryOp> PExpressionBinaryOp;


class ExpressionLogical : public Expression {
public:
    ExpressionLogical(
        std::string& value,
        const MiniJava::YYLTYPE pos
    ) : value_(value) { setPos(pos); }

    const std::string getValue() const { return value_; }
    void accept(IVisitor* visitor) const { visitor->visit(this); }
private:
    std::string value_;
};

typedef std::shared_ptr<ExpressionLogical> PExpressionLogical;


class ExpressionId : public Expression {
public:
    ExpressionId(
        const std::string& identifier,
        const MiniJava::YYLTYPE pos
    ) :
        id_(identifier) { setPos(pos); }

    const std::string& getId() const { return id_; };
    void accept(IVisitor* visitor) const { visitor->visit(this); }
private:
    std::string id_;
};

typedef std::shared_ptr<ExpressionId> PExpressionId;


class ExpressionSquareBracket: public Expression {
public:
    ExpressionSquareBracket(
        PExpression &entity,
        PExpression &index,
        const MiniJava::YYLTYPE pos
    ) :
        entity_(entity), index_(index) { setPos(pos); }

    const PExpression & getEntity() const { return entity_; };
    const PExpression & getIndex() const { return index_; };
    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    PExpression entity_;
    PExpression index_;
};

typedef std::shared_ptr<ExpressionSquareBracket> PExpressionSquareBracket;


class ExpressionLen: public Expression {
public:
    ExpressionLen(
        PExpression &arg,
        const MiniJava::YYLTYPE pos
    ) :
        arg_(arg) { setPos(pos); }

    const PExpression & getArg() const { return arg_; };
    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    PExpression arg_;
};

typedef std::shared_ptr<ExpressionLen> PExpressionLen;


class ExpressionUnaryNegation: public Expression {
public:
    ExpressionUnaryNegation (
        PExpression &arg,
        const MiniJava::YYLTYPE pos
    ) :
        arg_(arg) { setPos(pos); }

    const PExpression & getArg() const { return arg_; };
    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    PExpression arg_;
};

typedef std::shared_ptr<ExpressionUnaryNegation> PExpressionUnaryNegation;


class ExpressionThis : public Expression {
public:
    explicit ExpressionThis(
        const MiniJava::YYLTYPE pos
    ) { setPos(pos); }

    void accept(IVisitor* visitor) const override { visitor->visit(this); }
};

typedef std::shared_ptr<ExpressionThis> PExpressionThis;


class ExpressionNewId : public Expression {
public:
    ExpressionNewId(const std::string& identifier,
            const MiniJava::YYLTYPE pos
            ) :
                id_(identifier) { setPos(pos); }

    const std::string& getId() const { return id_; };
    void accept(IVisitor* visitor) const { visitor->visit(this);}
private:
    std::string id_;
};

typedef std::shared_ptr<ExpressionNewId> PExpressionNewId;

class ExpressionNewIntArray : public Expression {
public:
    ExpressionNewIntArray(
            PExpression &counter,
            const MiniJava::YYLTYPE pos
            ) : counter_(counter) { setPos(pos); }

    const PExpression & getCounter() const { return counter_; };
    void accept(IVisitor* visitor) const { visitor->visit(this);}
private:
    PExpression counter_;
};

typedef std::shared_ptr<ExpressionNewIntArray> PExpressionNewIntArray;


class ExpressionCallFunction : public Expression {
public:
    ExpressionCallFunction(
            PExpression& expr,
            const std::string& func_name,
            const std::vector<PExpression>& args,
            const MiniJava::YYLTYPE pos
            ) :  expr_(expr), func_name_(func_name), args_(args) { setPos(pos); }

    const PExpression & getExpr() const { return expr_; }
    const std::string & getFuncName() const { return func_name_; }
    const std::vector<PExpression> & getArgs() const { return args_; }
    void accept(IVisitor* visitor) const { visitor->visit(this);}
private:
    PExpression expr_;
    std::string func_name_;
    std::vector<PExpression> args_;
};

typedef std::shared_ptr<ExpressionCallFunction> PExpressionCallFunction;


}

