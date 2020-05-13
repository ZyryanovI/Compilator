#include "IRPrinter.h"
#include <cstdio>

IRPrinter::IRPrinter(std::string filename) {
    f = fopen(filename.c_str(), "w");
    nodeNumber = 0;
    fprintf(f, "%s", "strict graph G{\n");
}

IRPrinter::~IRPrinter() {
    fprintf(f, "%s", "}");
    fclose(f);
}

void IRPrinter::visit( const IRExpList* n){
    fprintf(f, " [label=\"IPLEMENT IRExpList! \\n\"];\n");
}

void IRPrinter::visit(const ConstExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"CONST\\n%d\"];\n", cur, n->value);
}

void IRPrinter::visit(const NameExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"NAME\\n%s\"];\n", cur, n->name.String().c_str());
}

void IRPrinter::visit(const TempExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"TEMP\\n%s\"];\n", cur, n->name.String().c_str());
}

void IRPrinter::visit(const BinOpExpression *n) {
    int cur = nodeNumber;
    std::string type = getBinop(n->binop);
    fprintf(f, "%d [label=\"%s\"];\n", cur, type.c_str());
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->left->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->right->Accept(this);
}

void IRPrinter::visit(const MemExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MEM\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr->Accept(this);
}

//void IRPrinter::visit(const CallExpression *n) {
//    int cur = nodeNumber;
//    fprintf(f, "%d [label=\"CALL\"];\n", cur);
//    nodeNumber++;
//    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
//    n->func->Accept(this);
////    const auto &arguments = n->args->Get();
//    const auto &arguments = n->args->GetCopy();
//
//    for (auto &arg : arguments) {
//        nodeNumber++;
//        fprintf(f, "%d -- %d;\n", cur, nodeNumber);
//        arg->Accept(this);
//    }
//}

void IRPrinter::visit(const CallExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"CALL\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->func->Accept(this);
    //std::vector< std::unique_ptr<const IRExpression> > copyList = n->args->Get(); //->Get();



    for (auto &arg : n->args->list) {
        nodeNumber++;
        fprintf(f, "%d -- %d;\n", cur, nodeNumber);
        arg->Accept(this);
    }
}



void IRPrinter::visit(const ESeqExpression *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"ESEQ\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->expr->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->stm->Accept(this);
}

void IRPrinter::visit(const MoveStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"MOVE\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->src->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->dst->Accept(this);
}

void IRPrinter::visit(const ExpStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"EXP\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->exp->Accept(this);
}

void IRPrinter::visit(const JumpStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"JUMP\\n%s\"];\n", cur, n->label.String().c_str());
}

void IRPrinter::visit(const CJumpStatement *n) {
    int cur = nodeNumber;
    std::string rel = getRel(n->rel);
    fprintf(f, "%d [label=\"CJUMP\\n%s\"];\n", cur, rel.c_str());
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->left->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->right->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    fprintf(f, "%d [label=\"iftrue\\n%s\"];\n", nodeNumber, n->if_left.String().c_str());
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    fprintf(f, "%d [label=\"iffalse\\n%s\"];\n", nodeNumber, n->if_right.String().c_str());
}

void IRPrinter::visit(const SeqStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"SEQ\"];\n", cur);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->left->Accept(this);
    nodeNumber++;
    fprintf(f, "%d -- %d;\n", cur, nodeNumber);
    n->right->Accept(this);
}

void IRPrinter::visit(const LabelStatement *n) {
    int cur = nodeNumber;
    fprintf(f, "%d [label=\"LABEL\\n%s\"];\n", cur, n->label.String().c_str());
}

std::string IRPrinter::getBinop(const BinOpExpression::BinOp rel) {
    if (rel == BinOpExpression::BinOp::PLUS) {
        return "+";
    } else if (rel == BinOpExpression::BinOp::MINUS) {
        return "-";
    } else if (rel == BinOpExpression::BinOp::MULT) {
        return "*";
    } else if (rel == BinOpExpression::BinOp::DIV) {
        return "/";
    } else if (rel == BinOpExpression::BinOp::REM) {
        return "%";
    } else if (rel == BinOpExpression::BinOp::LESS) {
        return "<";
    } else if (rel == BinOpExpression::BinOp::AND) {
        return "&&";
    } else {
        return "||";
    }
}

std::string IRPrinter::getRel(const CJumpStatement::Relation rel) {
    if (rel == CJumpStatement::Relation::EQ) {
        return "==";
    } else if (rel == CJumpStatement::Relation::NEQ) {
        return "!=";
    } else {
        return "<";
    }
}
