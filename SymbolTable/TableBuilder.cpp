#include "TableBuilder.h"
#include "Symbol.h"
#include <cassert>
#include <iostream>
#include <exception>

TableBuilder::TableBuilder()
    : table(new Table),
      curClass(nullptr),
      curMethod(nullptr),
      curVar(nullptr),
      curType(TYPE(NoneType{})),
      valExpr(true) {
}

bool TableBuilder::hasClass(Symbol *className) {
    return table->classes.find(className) != table->classes.end();
}

void TableBuilder::visit(const Goal *n) {
    for (auto &classDecl : *(n->class_declarations)) {
        classDecl->Accept(this);
    }
    n->main_class->Accept(this);

    for (auto &classDecl : *n->class_declarations) {
        curClass =
            table->classes[dynamic_cast<Identifier *>(
                               dynamic_cast<ClassDeclaration *>(classDecl.get())->class_name.get())
                               ->name];
        for (auto &cl : table->classes) {
            if (cl.second->parent != nullptr && cl.second->parent == curClass->name) {
                cl.second->parentInfo = curClass;
                for (auto &var : cl.second->vars) {
                    if (curClass->HasField(var.second->symbol)) {
                        errors.push_back(
                            "Error at line: " + std::to_string(var.second->first_line) +
                            " column: " + std::to_string(var.second->first_column) +
                            ". Message: variable " + var.first->getString() +
                            " already was declared\n");
                    }
                }

                for (auto &met : cl.second->methods) {
                    if (curClass->HasMethod(met.second->name)) {
                        auto method = curClass->GetMethod(met.second->name);
                        if (method->type->type != met.second->type->type ||
                            method->args.size() != met.second->args.size()) {
                            errors.push_back(
                                "Error at line: " + std::to_string(met.second->first_line) +
                                " column: " + std::to_string(met.second->first_column) +
                                ". Message: method " + met.first->getString() +
                                " already was declared.\n");
                        } else {
                            for (auto arg1 = method->args.begin(), arg2 = met.second->args.begin();
                                 arg1 != method->args.end(); arg1++, arg2++) {
                                if (arg1->second->type->type != arg2->second->type->type) {
                                    errors.push_back(
                                        "Error at line: " + std::to_string(met.second->first_line) +
                                        " column: " + std::to_string(met.second->first_column) +
                                        ". Message: method " + met.first->getString() +
                                        " already was declared.\n");
                                    break;
                                }
                            }
                        }
                    }
                    for (auto &arg : met.second->args) {
                        if (auto classVal = std::get_if<ClassType>(&(arg.second->type->type))) {
                            if (table->classes.find(classVal->name) == table->classes.end()) {
                                errors.push_back(
                                    "Error at line: " + std::to_string(arg.second->first_line) +
                                    " column: " + std::to_string(arg.second->first_column) +
                                    ". Message: type of argument " + arg.first->getString() +
                                    " was not declared yet\n");
                            }
                        }
                    }
                    if (auto classVal = std::get_if<ClassType>(&(met.second->type->type))) {
                        if (table->classes.find(classVal->name) == table->classes.end()) {
                            errors.push_back(
                                "Error at line: " + std::to_string(met.second->first_line) +
                                " column: " + std::to_string(met.second->first_column) +
                                ". Message: type of method " + met.first->getString() +
                                " was not declared yet.\n");
                        }
                    }
                }
            }
        }
        curClass = nullptr;
    }

    for (auto &classDecl : *n->class_declarations) {
        curClass =
            table->classes[dynamic_cast<Identifier *>(
                               dynamic_cast<ClassDeclaration *>(classDecl.get())->class_name.get())
                               ->name];
        if (curClass->parent != nullptr && curClass->parentInfo == nullptr) {
            errors.push_back("Error at line: " + std::to_string(curClass->first_line) +
                             " column: " + std::to_string(curClass->first_column) +
                             ". Message: parent of class " + curClass->name->getString() +
                             " wasn't declared yet.\n");
        } else if (curClass->parent != nullptr && curClass->parentInfo->parent == curClass->name) {
            errors.push_back("Error at line: " + std::to_string(curClass->first_line) +
                             " column: " + std::to_string(curClass->first_column) +
                             ". Message: cyclic inheritance.\n");
        }

        for (auto &met : curClass->methods) {
            for (auto &arg : met.second->args) {
                if (auto classVal = std::get_if<ClassType>(&(arg.second->type->type))) {
                    if (table->classes.find(classVal->name) == table->classes.end()) {
                        errors.push_back(
                            "Error at line: " + std::to_string(arg.second->first_line) +
                            " column: " + std::to_string(arg.second->first_column) +
                            ". Message: type of argument " + arg.first->getString() +
                            " was not declared yet.\n");
                    }
                }
            }
            if (auto classVal = std::get_if<ClassType>(&(met.second->type->type))) {
                if (table->classes.find(classVal->name) == table->classes.end()) {
                    errors.push_back("Error at line: " + std::to_string(met.second->first_line) +
                                     " column: " + std::to_string(met.second->first_column) +
                                     ". Message: type of method " + met.first->getString() +
                                     " was not declared yet.\n");
                }
            }
        }
        curClass = nullptr;
    }
}

void TableBuilder::visit(const MainClass *n) {
    auto name = dynamic_cast<Identifier *>(n->class_name.get())->name;
    curClass = new ClassInfo(name, nullptr, n->first_line, n->first_column);

    if (hasClass(curClass->name)) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: class " +
                         name->getString() + " was already declared.\n");
    }

    table->classes[curClass->name] = curClass;

    curMethod = new MethodInfo(nullptr, InternTable::getIntern("main"), 0, 0);
    curVar = new VariableInfo(nullptr, dynamic_cast<Identifier *>(n->arg.get())->name, 0, 0);
    curMethod->args[curVar->symbol] = curVar;
    curClass->methods[curMethod->name] = curMethod;
    n->statement->Accept(this);

    curClass = nullptr;
    curMethod = nullptr;
    curVar = nullptr;
}

void TableBuilder::visit(const ClassDeclaration *n) {
    auto name = dynamic_cast<Identifier *>(n->class_name.get())->name;
    curClass = new ClassInfo(name,
                             n->extends_class_name == nullptr
                                 ? nullptr
                                 : dynamic_cast<Identifier *>(n->extends_class_name.get())->name,
                             n->first_line, n->first_column);

    if (hasClass(curClass->name)) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: class " +
                         name->getString() + " was already declared.\n");
    }
    table->classes[curClass->name] = curClass;
    for (auto &varDeclaration : *(n->vars)) {
        varDeclaration->Accept(this);
    }
    for (auto methodDeclaration = n->methods->rbegin(); methodDeclaration != n->methods->rend();
         methodDeclaration++) {
        methodDeclaration->get()->Accept(this);
    }
    curClass = nullptr;
}

void TableBuilder::visit(const VarDeclaration *n) {
    auto name = dynamic_cast<Identifier *>(n->name.get())->name;
    curVar =
        new VariableInfo(dynamic_cast<Type *>(n->type.get()), name, n->first_line, n->first_column);
    if (curMethod != nullptr) {
        if (curMethod->locals.find(curVar->symbol) != curMethod->locals.end()) {
            errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                             std::to_string(n->first_column) + ". Message: variable " +
                             name->getString() + " was already declared as local var.\n");
        }
        curMethod->locals[curVar->symbol] = curVar;
    } else {
        if (curClass->HasField(curVar->symbol)) {
            errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                             std::to_string(n->first_column) + ". Message: variable " +
                             name->getString() + " was already declared as class field.\n");
        }
        curClass->vars[curVar->symbol] = curVar;
    }
    curVar = nullptr;
}

void TableBuilder::visit(const MethodDeclaration *n) {
    auto name = dynamic_cast<Identifier *>(n->name.get())->name;
    curMethod = new MethodInfo(dynamic_cast<Type *>(n->return_type.get()), name, n->first_line,
                               n->first_column);

    for (auto &var : *n->vars) {
        var->Accept(this);
    }

    for (auto &arg : *n->args) {
        curVar = new VariableInfo(dynamic_cast<Type *>(arg.first.get()),
                                  dynamic_cast<Identifier *>(arg.second.get())->name,
                                  dynamic_cast<Identifier *>(arg.second.get())->first_line,
                                  dynamic_cast<Identifier *>(arg.second.get())->first_column);
        if (curMethod->args.find(curVar->symbol) != curMethod->args.end()) {
            errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                             std::to_string(n->first_column) + ". Message: argument " +
                             dynamic_cast<Identifier *>(arg.second.get())->name->getString() +
                             " was already declared.\n");
        }
        curMethod->args[curVar->symbol] = curVar;
    }

    if (curClass->HasMethod(curMethod->name)) {
        auto method = curClass->GetMethod(curMethod->name);
        if (method->args.size() != curMethod->args.size()) {
            errors.push_back("Error at line: " + std::to_string(n->first_line) +
                             " column: " + std::to_string(n->first_column) + ". Message: method " +
                             name->getString() + " was already declared.\n");
        } else {
            for (auto arg1 = method->args.begin(), arg2 = curMethod->args.begin();
                 arg1 != method->args.end(); arg1++, arg2++) {
                if (arg1->second->type->type != arg2->second->type->type) {
                    errors.push_back("Error at line: " + std::to_string(n->first_line) +
                                     " column: " + std::to_string(n->first_column) +
                                     ". Message: method " + name->getString() +
                                     " already was declared.\n");
                    break;
                }
            }
        }
    }

    curClass->methods[curMethod->name] = curMethod;

    for (auto &stat : *n->statements) {
        stat->Accept(this);
    }

    n->return_expression->Accept(this);
    if (valExpr && curType != *(curMethod->type)) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: returned type doesn't match function signature.\n");
    }
    valExpr = true;
    curType = Type(NoneType{});
    curMethod = nullptr;
}

void TableBuilder::visit(const Type *) {
    assert(false);
}

void TableBuilder::visit(const IfStatement *n) {
    n->clause->Accept(this);
    if (valExpr && curType != Type(BoolType{})) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: bool type is required.\n");
    }
    valExpr = true;
    curType = Type(NoneType{});
    n->true_statement->Accept(this);
    n->false_statement->Accept(this);
}

void TableBuilder::visit(const WhileStatement *n) {
    n->clause->Accept(this);
    if (valExpr && curType != Type(BoolType{})) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: bool type is required.\n");
    }
    valExpr = true;
    curType = Type(NoneType{});
    n->body->Accept(this);
}

void TableBuilder::visit(const Statement *n) {
    for (auto &stat : *(n->statements)) {
        stat->Accept(this);
    }
}

void TableBuilder::visit(const PrintStatement *n) {
    n->print->Accept(this);
    if (valExpr && curType != Type(IntType{})) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: wrong type for printing.\n");
    }
}

void TableBuilder::visit(const AssignmentStatement *n) {
    n->expr->Accept(this);
    auto name = dynamic_cast<Identifier *>(n->var.get())->name;
    if (curMethod->locals.find(name) != curMethod->locals.end()) {
        auto var = curMethod->locals.find(name)->second;
        if (valExpr && curType != *(var->type)) {
            errors.push_back("Error at line: " + std::to_string(n->first_line) +
                             " column: " + std::to_string(n->first_column) +
                             ". Message: type mismatch while assigning.\n");
        }
    } else if (curClass->vars.find(name) != curClass->vars.end()) {
        auto var = curClass->vars.find(name)->second;
        if (valExpr && curType != *(var->type)) {
            errors.push_back("Error at line: " + std::to_string(n->first_line) +
                             " column: " + std::to_string(n->first_column) +
                             ". Message: type mismatch while assigning.\n");
        }
    } else {
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: variable " +
                         name->getString() + " doesn't exist\n");
    }
    valExpr = true;
    curType = Type(NoneType{});
}

void TableBuilder::visit(const ArrAssignmentStatement *n) {
    n->num->Accept(this);
    if (valExpr && curType != Type(IntType{})) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: int array type is required.\n");
    }
    valExpr = true;
    curType = Type(NoneType{});

    n->expr->Accept(this);
    if (valExpr && curType != Type(IntType{})) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    valExpr = true;
    curType = Type(NoneType{});
    auto name = dynamic_cast<Identifier *>(n->var.get())->name;
    if (curMethod->locals.find(name) == curMethod->locals.end() &&
        curClass->vars.find(name) == curClass->vars.end()) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: variable " +
                         name->getString() + " doesn't exist\n");
    }
}

void TableBuilder::visit(const AndExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(BoolType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(BoolType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(BoolType{});
}

void TableBuilder::visit(const LessExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(BoolType{});
}

void TableBuilder::visit(const PlusExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(IntType{});
}

void TableBuilder::visit(const MinusExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(IntType{});
}

void TableBuilder::visit(const MultExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(IntType{});
}

void TableBuilder::visit(const OrExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(BoolType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: boolean type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(BoolType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: boolean type is required.\n");
    }
    curType = Type(BoolType{});
}

void TableBuilder::visit(const RemainExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(IntType{});
}

void TableBuilder::visit(const ArrayExpression *n) {
    n->expr1->Accept(this);
    if (curType != Type(IntArrType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: int array type is required.\n");
    }
    curType = Type(NoneType{});
    n->expr2->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(IntType{});
}

void TableBuilder::visit(const LengthExpression *n) {
    n->expr->Accept(this);
    if (curType != Type(IntArrType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: int array type is required.\n");
    }
    curType = Type(IntType{});
}

void TableBuilder::visit(const MethodExpression *n) {
    n->class_name->Accept(this);
    auto name = dynamic_cast<Identifier *>(n->method.get())->name;
    if (curType == Type(IntType{}) || curType == Type(NoneType{}) ||
        curType == Type(IntArrType{}) || curType == Type(BoolType{})) {
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: calling a method " +
                         name->getString() + " from a primitive type.\n");
        valExpr = false;
        return;
    }
    if (curClass->methods.find(name) == curClass->methods.end()) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: method " +
                         name->getString() + " doesn't exist.\n");
        return;
    }
    auto met = curClass->methods[name];
    if (met->args.size() != n->params->size()) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: method " +
                         name->getString() + " was called with wrong number of args\n");
        return;
    }
    curType = *(met->type);
}

void TableBuilder::visit(const Integer *) {
    curType = Type(IntType{});
}

void TableBuilder::visit(const Bool *) {
    curType = Type(BoolType{});
}

void TableBuilder::visit(const IdentExpression *n) {
    n->ident->Accept(this);
}

void TableBuilder::visit(const This *) {
    curType = Type(ClassType{curClass->name});
}

void TableBuilder::visit(const NewArrExpression *n) {
    n->expr->Accept(this);
    if (curType != Type(IntType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) + " column: " +
                         std::to_string(n->first_column) + ". Message: int type is required.\n");
    }
    curType = Type(IntArrType{});
}

void TableBuilder::visit(const NewExpression *n) {
    n->ident->Accept(this);
    if (curType == Type(IntType{}) || curType == Type(NoneType{}) ||
        curType == Type(IntArrType{}) || curType == Type(BoolType{})) {
        valExpr = false;
    }
}

void TableBuilder::visit(const NotExpression *n) {
    n->expr->Accept(this);
    if (curType != Type(BoolType{})) {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) +
                         ". Message: boolean type is required.\n");
    }
    curType = Type(BoolType{});
}

void TableBuilder::visit(const Expression *n) {
    n->expr->Accept(this);
}

void TableBuilder::visit(const Identifier *n) {
    if (curClass->vars.find(n->name) != curClass->vars.end()) {
        auto var = curClass->vars[n->name];
        curType = *(var->type);
    } else if (curMethod->locals.find(n->name) != curMethod->locals.end()) {
        auto var = curMethod->locals[n->name];
        curType = *(var->type);
    } else if (curMethod->args.find(n->name) != curMethod->args.end()) {
        auto var = curMethod->args[n->name];
        curType = *(var->type);
    } else if (table->classes.find(n->name) != table->classes.end()) {
        auto cl = table->classes[n->name];
        curType = Type(ClassType{cl->name});
        curClass = cl;
    } else {
        valExpr = false;
        errors.push_back("Error at line: " + std::to_string(n->first_line) +
                         " column: " + std::to_string(n->first_column) + ". Message: name " +
                         n->name->getString() + " wasn't declared.\n");
    }
}

void TableBuilder::printErrors() {
    for (auto &err : errors) {
        std::cout << err;
    }
    if (errors.size() > 0) {
        throw std::exception();
    }
}
