#include "Statements.h"
#include "../Visitor.h"
#include <iostream>

Statement::Statement(int first_line, int first_column, std::vector<std::unique_ptr<IStatement>> *statements) {
    if (statements == nullptr) {
        std::cout << "Statement is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->statements = std::unique_ptr<std::vector<std::unique_ptr<IStatement>>>(statements);
}

void Statement::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

IfStatement::IfStatement(int first_line, int first_column, IExpression *clause, IStatement *true_statement, IStatement *false_statement) {
    if (clause == nullptr || true_statement == nullptr || false_statement == nullptr) {
        std::cout << "Statement is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->clause = std::unique_ptr<IExpression>(clause);
    this->true_statement = std::unique_ptr<IStatement>(true_statement);
    this->false_statement = std::unique_ptr<IStatement>(false_statement);
}

void IfStatement::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

WhileStatement::WhileStatement(int first_line, int first_column, IExpression *clause, IStatement *body) {
    if (clause == nullptr || body == nullptr) {
        std::cout << "Statement is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->clause = std::unique_ptr<IExpression>(clause);
    this->body = std::unique_ptr<IStatement>(body);
}

void WhileStatement::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

PrintStatement::PrintStatement(int first_line, int first_column, IExpression *print) {
    this->first_line = first_line;
    this->first_column = first_column;
    this->print = std::unique_ptr<IExpression>(print);
}

void PrintStatement::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

AssignmentStatement::AssignmentStatement(int first_line, int first_column, IIdentifier *var, IExpression *expr) {
    this->first_line = first_line;
    this->first_column = first_column;
    this->var = std::unique_ptr<IIdentifier>(var);
    this->expr = std::unique_ptr<IExpression>(expr);
}

void AssignmentStatement::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

ArrAssignmentStatement::ArrAssignmentStatement(int first_line, int first_column, IIdentifier *var, IExpression *num, IExpression *expr) {
    this->first_line = first_line;
    this->first_column = first_column;
    this->var = std::unique_ptr<IIdentifier>(var);
    this->num = std::unique_ptr<IExpression>(num);
    this->expr = std::unique_ptr<IExpression>(expr);
}

void ArrAssignmentStatement::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

