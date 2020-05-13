#ifndef AST_Expression
#define AST_Expression

#include "INode.h"
#include <vector>
#include <memory>

class AndExpression : public IExpression {
public:
    AndExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class LessExpression : public IExpression {
public:
    LessExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class PlusExpression : public IExpression {
public:
    void Accept(Visitor* v) const;
    PlusExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class MinusExpression : public IExpression {
public:
    MinusExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class MultExpression : public IExpression {
public:
    MultExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class RemainExpression : public IExpression {
public:
    RemainExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class OrExpression : public IExpression {
public:
    OrExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class ArrayExpression : public IExpression {
public:
    ArrayExpression(int first_line, int first_column, IExpression* expr1, IExpression* expr2);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;
};

class LengthExpression : public IExpression {
public:
    LengthExpression(int first_line, int first_column, IExpression* expr);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr;
};

class MethodExpression : public IExpression {
public:
    MethodExpression(int first_line, int first_column, IExpression* class_name, IIdentifier* method,
                     std::vector<std::unique_ptr<IExpression>>* params);
    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> class_name;
    std::unique_ptr<IIdentifier> method;
    std::unique_ptr<std::vector<std::unique_ptr<IExpression>>> params;
};

class Integer : public IExpression {
public:
    Integer(int first_line, int first_column, int num);
    void Accept(Visitor* v) const;

public:
    int num;
};

class Bool : public IExpression {
public:
    Bool(int first_line, int first_column, bool b);

    void Accept(Visitor* v) const;

public:
    bool b;
};

class IdentExpression : public IExpression {
public:
    IdentExpression(int first_line, int first_column, IIdentifier* ident);

    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IIdentifier> ident;
};

class This : public IExpression {
public:
    This(int first_line, int first_column);
    void Accept(Visitor* v) const;
};

class NewArrExpression : public IExpression {
public:
    NewArrExpression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr;
};

class NewExpression : public IExpression {
public:
    NewExpression(int first_line, int first_column, IIdentifier* ident);

    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IIdentifier> ident;
};

class NotExpression : public IExpression {
public:
    NotExpression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr;
};

class Expression : public IExpression {
public:
    Expression(int first_line, int first_column, IExpression* expr);

    void Accept(Visitor* v) const;

public:
    std::unique_ptr<IExpression> expr;
};

#endif  // AST_Expression
