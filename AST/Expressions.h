#pragma once

#include "Interfaces.h"
#include <vector>
#include <memory>

class AndExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    AndExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* visitor) const;
};

class LessExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    LessExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* visitor) const;
};

class PlusExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    PlusExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* visitor) const;
};

class MinusExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    MinusExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* visitor) const;
};

class MultExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    MultExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* visitor) const;
};


class ArrayExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    ArrayExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* visitor) const;
};

class LengthExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

    LengthExpression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* visitor) const;
};

class MethodExpression : public IExpression {
public:
    std::unique_ptr<IExpression> class_name;
    std::unique_ptr<IIdentifier> method;
    std::unique_ptr<std::vector<std::unique_ptr<IExpression>>> params;

    MethodExpression(int first_line, int first_column, IExpression* class_name, IIdentifier* method,
                     std::vector<std::unique_ptr<IExpression>>* params);

    void Accept(Visitor* visitor) const;
};

class Integer : public IExpression {
public:
    int num;

    Integer(int first_line, int first_column, int num);

    void Accept(Visitor* visitor) const;
};

class Bool : public IExpression {
public:
    bool b;

    Bool(int first_line, int first_column, bool b);

    void Accept(Visitor* visitor) const;
};

class IdentExpression : public IExpression {
public:
    std::unique_ptr<IIdentifier> ident;

    IdentExpression(int first_line, int first_column, IIdentifier* ident);

    void Accept(Visitor* visitor) const;
};

class This : public IExpression {
public:
    This(int first_line, int first_column);

    void Accept(Visitor* visitor) const;
};

class NewArrExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

    NewArrExpression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* visitor) const;
};

class NewExpression : public IExpression {
public:
    std::unique_ptr<IIdentifier> ident;

    NewExpression(int first_line, int first_column, IIdentifier* ident);

    void Accept(Visitor* visitor) const;
};

class NotExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

    NotExpression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* visitor) const;
};

class Expression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

    Expression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* visitor) const;
};

