#include <iostream>
#include "Expressions.h"
#include "../Visitor.h"

AndExpression::AndExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

void AndExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

LessExpression::LessExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

void LessExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

PlusExpression::PlusExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

void PlusExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

MinusExpression::MinusExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

void MinusExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

MultExpression::MultExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

void MultExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}


ArrayExpression::ArrayExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

void ArrayExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

LengthExpression::LengthExpression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}

void LengthExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

MethodExpression::MethodExpression(int first_line, int first_column, IExpression *class_name,
                                   IIdentifier *method, std::vector<std::unique_ptr<IExpression>> *params) {
    if (class_name == nullptr || method == nullptr || params == nullptr) {
        std::cout << "While initializing MethodExpression, one of params is nullptr" <<std::endl;
    }

    this->first_line = first_line;
    this->first_column = first_column;
    this->class_name = std::unique_ptr<IExpression>(class_name);
    this->method = std::unique_ptr<IIdentifier>(method);
    this->params = std::unique_ptr<std::vector<std::unique_ptr<IExpression>>>(params);
}

void MethodExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

Integer::Integer(int first_line, int first_column, int num) : num(num) {
    this->first_line = first_line;
    this->first_column = first_column;
}

void Integer::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

Bool::Bool(int first_line, int first_column, bool b) : b(b) {
    this->first_line = first_line;
    this->first_column = first_column;
}

void Bool::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

IdentExpression::IdentExpression(int first_line, int first_column, IIdentifier *ident) {
    if (ident == nullptr) {
        std::cout << "While initializing IdentExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->ident = std::unique_ptr<IIdentifier>(ident);
}

void IdentExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

NewArrExpression::NewArrExpression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "While initializing NewArrExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}

void NewArrExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

NewExpression::NewExpression(int first_line, int first_column, IIdentifier *ident) {
    if (ident == nullptr) {
        std::cout << "While initializing NewExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->ident = std::unique_ptr<IIdentifier>(ident);
}

void NewExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

NotExpression::NotExpression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "While initializing NotExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}

void NotExpression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

Expression::Expression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "While initializing Expression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}

void Expression::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

This::This(int first_line, int first_column) {
    this->first_line = first_line;
    this->first_column = first_column;
}

void This::Accept(Visitor* visitor) const {
    visitor->visit(this);
}
