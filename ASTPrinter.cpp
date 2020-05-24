#include "ASTPrinter.h"
#include <cstdio>
#include <variant>

ASTPrinter::ASTPrinter(std::string outFile) {
    f = fopen(outFile.c_str(), "w");
    nodeNumber = 0;
    fprintf(f, "%s", "graph Grammar{\n");
}

ASTPrinter::~ASTPrinter() {
    fprintf(f, "%s", "}");
    fclose(f);
}

void ASTPrinter::visit(const Goal *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"Goal\"] [color=blue];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->main_class->Accept(this);
    for (size_t i = 0; i < n->class_declarations->size(); i++) {
        auto &ptr = (*n->class_declarations)[i];
        if (ptr) {
            nodeNumber++;
            fprintf(f, "%d -- %d;\n", cur, nodeNumber);
            ptr->Accept(this);
        } else {
            throw std::exception();
        }
    }
}

void ASTPrinter::visit(const MainClass *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MainClass\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->class_name->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->arg->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->statement->Accept(this);
}

void ASTPrinter::visit(const ClassDeclaration *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"ClassDeclaration\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->class_name->Accept(this);
    if (n->extends_class_name) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        n->extends_class_name->Accept(this);
    }
    for (size_t i = 0; i < n->vars->size(); i++) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        (*(n->vars))[i]->Accept(this);
    }
    for (size_t i = 0; i < n->methods->size(); i++) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        (*(n->methods))[i]->Accept(this);
    }
}

void ASTPrinter::visit(const VarDeclaration *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"VarDeclaration\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->type->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->name->Accept(this);
}

void ASTPrinter::visit(const MethodDeclaration *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MethodDeclaration\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->return_type->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->name->Accept(this);
    for (size_t i = 0; i < n->args->size(); i++) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);
        int argNumber = nodeNumber;
        fprintf(f, "%d [label=\"Argument\"];\n", argNumber);
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", argNumber, nodeNumber);
        ((*(n->args))[i]).first->Accept(this);  // type

        nodeNumber++;
        fprintf(f, "%d -- %d;\n", argNumber, nodeNumber);
        ((*(n->args))[i]).second->Accept(this);  // name
    }

    for (size_t i = 0; i < n->vars->size(); i++) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        (*(n->vars))[i]->Accept(this);
    }
    for (size_t i = 0; i < n->statements->size(); i++) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        (*(n->statements))[i]->Accept(this);
    }
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    fprintf(f, "%d [label=\"Return\"];\n", nodeNumber);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", nodeNumber - 1, nodeNumber);

    n->return_expression->Accept(this);
}

void ASTPrinter::visit(const Type *n) {
    int cur = nodeNumber;
    if (std::get_if<IntType>(&(n->type))) {
        fprintf(f, "%d [label=\"IntType\"];\n", cur);

        nodeNumber++;
        fprintf(f, "%d [label=\"int\"];\n", nodeNumber);
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    } else if (std::get_if<BoolType>(&(n->type))) {
        fprintf(f, "%d [label=\"BoolType\"];\n", cur);

        nodeNumber++;
        fprintf(f, "%d [label=\"boolean\"];\n", nodeNumber);
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    } else if (std::get_if<IntArrType>(&(n->type))) {
        fprintf(f, "%d [label=\"IntArrayType\"];\n", cur);

        nodeNumber++;
        fprintf(f, "%d [label=\"int[]\"];\n", nodeNumber);
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    } else {
        auto classVal = std::get_if<ClassType>(&(n->type));
        fprintf(f, "%d [label=\"Type\"];\n", cur);
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);
        fprintf(f, "%d [label=\"%s\"];\n", nodeNumber, classVal->name->getString().c_str());
    }
}

void ASTPrinter::visit(const Statement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"Statement\"];\n", cur);

    for (size_t i = 0; i < n->statements->size(); i++) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        (*(n->statements))[i]->Accept(this);
    }
}

void ASTPrinter::visit(const IfStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"IfStatement\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->clause->Accept(this);

    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->true_statement->Accept(this);

    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->false_statement->Accept(this);
}

void ASTPrinter::visit(const WhileStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"WhileStatement\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->clause->Accept(this);

    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->body->Accept(this);
}

void ASTPrinter::visit(const PrintStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"PrintStatement\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->print->Accept(this);
}

void ASTPrinter::visit(const AssignmentStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"AssignmentStatement\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->var->Accept(this);

    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr->Accept(this);
}

void ASTPrinter::visit(const ArrAssignmentStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"ArrayAssignmentStatement\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->var->Accept(this);

    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->num->Accept(this);

    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr->Accept(this);
}

void ASTPrinter::visit(const AndExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"AND\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr1->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr2->Accept(this);
}

void ASTPrinter::visit(const LessExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"LESS\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr1->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr2->Accept(this);
}

void ASTPrinter::visit(const PlusExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"PLUS\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr1->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr2->Accept(this);
}

void ASTPrinter::visit(const MinusExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MINUS\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr1->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr2->Accept(this);
}

void ASTPrinter::visit(const MultExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MULT\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr1->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr2->Accept(this);
}

void ASTPrinter::visit(const ArrayExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"ArrayExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->expr1->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->expr2->Accept(this);
}

void ASTPrinter::visit(const LengthExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"LengthExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->expr->Accept(this);
}

void ASTPrinter::visit(const MethodExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MethodCallExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->class_name->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->method->Accept(this);
    for (size_t i = 0; i < n->params->size(); ++i) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);

        (*n->params)[i]->Accept(this);
    }
}

void ASTPrinter::visit(const Integer *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"Integer\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    fprintf(f, "%d [label=\"%d\"];\n", nodeNumber, n->num);
}

void ASTPrinter::visit(const Bool *n) {

    int cur = nodeNumber;
    fprintf(f, "%d [label=\"Bool\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    fprintf(f, "%d [label=\"%s\"];\n", nodeNumber, n->b ? "true" : "false");
}

void ASTPrinter::visit(const IdentExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"IdentExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->ident->Accept(this);
}

void ASTPrinter::visit(const This *) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"This\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    fprintf(f, "%d [label=\"this\"];\n", nodeNumber);
}

void ASTPrinter::visit(const NewArrExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"NewArrExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->expr->Accept(this);
}

void ASTPrinter::visit(const NewExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"NewExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->ident->Accept(this);
}

void ASTPrinter::visit(const NotExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"NotExpression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->expr->Accept(this);
}

void ASTPrinter::visit(const Expression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"Expression\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);

    n->expr->Accept(this);
}

void ASTPrinter::visit(const Identifier *n) {
    fprintf(f, "%d [label=\"%s\"];\n", nodeNumber, n->name->getString().c_str());
}