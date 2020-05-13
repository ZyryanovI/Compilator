#include "Printer.h"
#include <cstdio>

Printer::Printer(FILE *outFile) {
    f = outFile;
}

void Printer::visit(const Goal *n) {
    n->main_class->Accept(this);
    for (size_t i = 0; i < n->class_declarations->size(); i++) {
        auto &ptr = (*n->class_declarations)[i];
        if (ptr) {
            ptr->Accept(this);
        } else {
            throw std::exception();
        }
    }
}

void Printer::visit(const MainClass *n) {
    fprintf(f, "class ");
    n->class_name->Accept(this);
    fprintf(f, " { \n public static void main(String[] ");
    n->arg->Accept(this);
    fprintf(f, ") { \n");
    n->statement->Accept(this);
    fprintf(f, "} } \n");
}

void Printer::visit(const ClassDeclaration *n) {
    fprintf(f, "class ");
    n->class_name->Accept(this);
    if (n->extends_class_name) {
        fprintf(f, " extends ");
        n->extends_class_name->Accept(this);
    }
    fprintf(f, " { \n");
    for (size_t i = 0; i < n->vars->size(); i++) {
        (*(n->vars))[i]->Accept(this);
    }
    for (size_t i = 0; i < n->methods->size(); i++) {
        (*(n->methods))[i]->Accept(this);
    }
    fprintf(f, "} \n");
}

void Printer::visit(const VarDeclaration *n) {
    n->type->Accept(this);
    n->name->Accept(this);
    fprintf(f, "; \n");
}

void Printer::visit(const MethodDeclaration *n) {
    fprintf(f, "public ");
    n->return_type->Accept(this);
    n->name->Accept(this);
    fprintf(f, "(");
    for (size_t i = 0; i < n->args->size(); i++) {
        ((*(n->args))[i]).first->Accept(this);
        ((*(n->args))[i]).second->Accept(this);
        if (i < n->args->size() - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, ") { \n");
    for (size_t i = 0; i < n->vars->size(); i++) {
        (*(n->vars))[i]->Accept(this);
    }
    for (size_t i = 0; i < n->statements->size(); i++) {
        (*(n->statements))[i]->Accept(this);
    }
    fprintf(f, "return ");
    n->return_expression->Accept(this);
    fprintf(f, "; } \n");
}


void Printer::visit(const IntType *) {
    fprintf(f, "int ");
}

void Printer::visit(const BoolType *) {
    fprintf(f, "boolean ");
}

void Printer::visit(const IntArrayType *) {
    fprintf(f, "int[] ");
}

void Printer::visit(const Type *n) {
    n->name->Accept(this);
    fprintf(f, " ");
}

void Printer::visit(const Statement *n) {
    fprintf(f, "{ ");
    for (size_t i = 0; i < n->statements->size(); i++) {
        (*(n->statements))[i]->Accept(this);
    }
    fprintf(f, "} \n");
}

void Printer::visit(const IfStatement *n) {
    fprintf(f, "if(");
    n->clause->Accept(this);
    fprintf(f, ") { \n");
    n->true_statement->Accept(this);
    fprintf(f, "} else { \n");
    n->false_statement->Accept(this);
    fprintf(f, "} \n");
}

void Printer::visit(const WhileStatement *n) {
    fprintf(f, "while(");
    n->clause->Accept(this);
    fprintf(f, ") { \n");
    n->body->Accept(this);
    fprintf(f, "} \n");
}

void Printer::visit(const PrintStatement *n) {
    fprintf(f, "System.out.println(");
    n->print->Accept(this);
    fprintf(f, "); \n");
}

void Printer::visit(const AssignmentStatement *n) {
    n->var->Accept(this);
    fprintf(f, " = ");
    n->expr->Accept(this);
    fprintf(f, "; \n");
}

void Printer::visit(const ArrAssignmentStatement *n) {
    n->var->Accept(this);
    fprintf(f, "[");
    n->num->Accept(this);
    fprintf(f, "] = ");
    n->expr->Accept(this);
    fprintf(f, "; \n");
}


void Printer::visit(const AndExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " AND ");
    n->expr2->Accept(this);
}

void Printer::visit(const LessExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " LESS ");
    n->expr2->Accept(this);
}


void Printer::visit(const PlusExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " PLUS ");
    n->expr2->Accept(this);
}

void Printer::visit(const MinusExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " MINUS ");
    n->expr2->Accept(this);
}

void Printer::visit(const MultExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " MULT ");
    n->expr2->Accept(this);
}

void Printer::visit(const OrExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " OR ");
    n->expr2->Accept(this);
}

void Printer::visit(const RemainExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, " Remain ");
    n->expr2->Accept(this);
}

void Printer::visit(const ArrayExpression *n) {
    n->expr1->Accept(this);
    fprintf(f, "[");
    n->expr2->Accept(this);
    fprintf(f, "]");
}

void Printer::visit(const LengthExpression *n) {
    n->expr->Accept(this);
    fprintf(f, ".length");
}

void Printer::visit(const MethodExpression *n) {
    n->class_name->Accept(this);
    fprintf(f, ".");
    n->method->Accept(this);
    fprintf(f, "(");
    for (size_t i = 0; i < n->params->size(); ++i) {
        (*n->params)[i]->Accept(this);
        if (i != n->params->size() - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, ")");
}

void Printer::visit(const Integer *n) {
    fprintf(f, "%d", n->num);
}

void Printer::visit(const Bool *n) {
    fprintf(f, "%s", n->b ? "true" : "false");
}

void Printer::visit(const IdentExpression *n) {
    n->ident->Accept(this);
}

void Printer::visit(const This *) {
    fprintf(f, "this");
}

void Printer::visit(const NewArrExpression *n) {
    fprintf(f, "new int[");
    n->expr->Accept(this);
    fprintf(f, "]");
}

void Printer::visit(const NewExpression *n) {
    fprintf(f, "new ");
    n->ident->Accept(this);
    fprintf(f, "()");
}

void Printer::visit(const NotExpression *n) {
    fprintf(f, "!");
    n->expr->Accept(this);
}

void Printer::visit(const Expression *n) {
    fprintf(f, "(");
    n->expr->Accept(this);
    fprintf(f, ")");
}

void Printer::visit(const Identifier *n) {
    fprintf(f, "%s", n->name->getString().c_str());
}
