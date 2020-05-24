#include "Symbol.hpp"
#include "Table.hpp"
#include "ClassInfo.hpp"
#include "MethodInfo.hpp"
#include "VarInfo.hpp"
#include "../Visitor.hpp"

#include <vector>
#include <string>

class TableBuilder : Visitor {
private:
    Table* table;
    ClassInfo* curClass;
    MethodInfo* curMethod;
    VariableInfo* curVar;
    Type curType;
    bool isvalidExpr;
    std::vector<std::string> errors;

public:

    TableBuilder(): table(new Table), curClass(nullptr), curMethod(nullptr),
          curVar(nullptr), curType(TYPE(NoneType{})), isvalidExpr(true) {}

    Table* buildTable(Goal* goal) {
        visit(goal);
        printErrors();
        return table;
    }

    void printErrors() {
        for (auto &err : errors) {
            std::cout << err;
        }
        if (!errors.empty()) {
            throw std::exception();
        }
    }

    bool hasClass(Symbol* className);

    void visit(const Goal *goal) override {

        for (auto &classDecl : *(goal->class_declarations)) {
            classDecl->Accept(this);
        }

        goal->main_class->Accept(this);

        for (auto &classDecl : *goal->class_declarations) {
            curClass = table->classes[dynamic_cast<Identifier *>(dynamic_cast<ClassDeclaration *>(classDecl.get())->class_name.get())->name];

            for (auto &cl : table->classes) {
                if (cl.second->parent != nullptr && cl.second->parent == curClass->name) {
                    cl.second->parentInfo = curClass;
                    for (auto &var : cl.second->vars) {
                        if (curClass->HasField(var.second->symbol)) {
                            errors.push_back("Error at line: " + std::to_string(var.second->first_line) + " column: " + std::to_string(var.second->first_column) +
                                    ". Message: variable " + var.first->getString() + " already was declared\n");
                        }
                    }

                    for (auto &met : cl.second->methods) {
                        if (curClass->HasMethod(met.second->name)) {
                            auto method = curClass->GetMethod(met.second->name);
                            if (method->type->type != met.second->type->type || method->args.size() != met.second->args.size()) {
                                errors.push_back("Error at line: " + std::to_string(met.second->first_line) + " column: " + std::to_string(met.second->first_column) +
                                        ". Message: method " + met.first->getString() + " already was declared.\n");
                            } else {
                                for (auto arg1 = method->args.begin(), arg2 = met.second->args.begin(); arg1 != method->args.end(); arg1++, arg2++) {
                                    if (arg1->second->type->type != arg2->second->type->type) {
                                        errors.push_back("Error at line: " + std::to_string(met.second->first_line) +
                                                " column: " + std::to_string(met.second->first_column) +
                                                ". Message: method " + met.first->getString() +
                                                " already was declared.\n");
                                        break;
                                    }
                                }
                            }
                        }
                        for (auto &arg : met.second->args) {
                            //проверка что есть типы у аргумнетов
                            if (auto classVal = std::get_if<ClassType>(&(arg.second->type->type))) { //если тип = ClassType
                                if (table->classes.find(classVal->name) == table->classes.end()) {
                                    errors.push_back("Error at line: " + std::to_string(arg.second->first_line) +
                                            " column: " + std::to_string(arg.second->first_column) +
                                            ". Message: type of argument " + arg.first->getString() +
                                            " was not declared yet\n");
                                }
                            }
                        }
                        //проверка что есть тип возвращаемый
                        if (auto classVal = std::get_if<ClassType>(&(met.second->type->type))) {
                            if (table->classes.find(classVal->name) == table->classes.end()) {
                                errors.push_back("Error at line: " + std::to_string(met.second->first_line) +
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

        for (auto &classDecl : *goal->class_declarations) {
            curClass = table->classes[dynamic_cast<Identifier *>(dynamic_cast<ClassDeclaration *>(classDecl.get())->class_name.get())->name];
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
                            errors.push_back("Error at line: " + std::to_string(arg.second->first_line) +
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
    
    void visit(const MainClass *mainClass) override {
        auto name = dynamic_cast<Identifier *>(mainClass->class_name.get())->name;
        curClass = new ClassInfo(name, nullptr, mainClass->first_line, mainClass->first_column);

        if (hasClass(curClass->name)) {
            errors.push_back("Error at line: " + std::to_string(mainClass->first_line) +
                             " column: " + std::to_string(mainClass->first_column) + ". Message: class " +
                             name->getString() + " was already declared.\n");
        }

        table->classes[curClass->name] = curClass;

        curMethod = new MethodInfo(nullptr, InternTable::getIntern("main"), 0, 0);
        curVar = new VariableInfo(nullptr, dynamic_cast<Identifier *>(mainClass->arg.get())->name, 0, 0);
        curMethod->args[curVar->symbol] = curVar;
        curClass->methods[curMethod->name] = curMethod;
        mainClass->statement->Accept(this);

        curClass = nullptr;
        curMethod = nullptr;
        curVar = nullptr;
    }

    void visit(const ClassDeclaration *classDeclaration) override {
        auto name = dynamic_cast<Identifier *>(classDeclaration->class_name.get())->name;
        curClass = new ClassInfo(name,classDeclaration->extends_class_name == nullptr ? nullptr : dynamic_cast<Identifier *>(classDeclaration->extends_class_name.get())->name,
                                 classDeclaration->first_line, classDeclaration->first_column);

        if (hasClass(curClass->name)) {
            errors.push_back("Error at line: " + std::to_string(classDeclaration->first_line) +
                             " column: " + std::to_string(classDeclaration->first_column) + ". Message: class " +
                             name->getString() + " was already declared.\n");
        }

        table->classes[curClass->name] = curClass;
        for (auto &varDeclaration : *(classDeclaration->vars)) {
            varDeclaration->Accept(this);
        }
        for (auto methodDeclaration = classDeclaration->methods->rbegin(); methodDeclaration != classDeclaration->methods->rend(); methodDeclaration++) { //обходим в обратном порядке
            methodDeclaration->get()->Accept(this);
        }
        curClass = nullptr;
    }

    void visit(const VarDeclaration *varDeclaration) override {
        auto name = dynamic_cast<Identifier *>(varDeclaration->name.get())->name;
        curVar = new VariableInfo(dynamic_cast<Type *>(varDeclaration->type.get()), name, varDeclaration->first_line, varDeclaration->first_column);

        if (curMethod != nullptr) {
            if (curMethod->locals.find(curVar->symbol) != curMethod->locals.end()) {
                errors.push_back("Error at line: " + std::to_string(varDeclaration->first_line) + " column: " +
                                 std::to_string(varDeclaration->first_column) + ". Message: variable " +
                                 name->getString() + " was already declared as local var.\n");
            }
            curMethod->locals[curVar->symbol] = curVar;

        } else {
            if (curClass->HasField(curVar->symbol)) {
                errors.push_back("Error at line: " + std::to_string(varDeclaration->first_line) + " column: " +
                                 std::to_string(varDeclaration->first_column) + ". Message: variable " +
                                 name->getString() + " was already declared as class field.\n");
            }
            curClass->vars[curVar->symbol] = curVar;
        }
        curVar = nullptr;
    }

    void visit(const MethodDeclaration *methodDeclaration) override {
        auto name = dynamic_cast<Identifier *>(methodDeclaration->name.get())->name;

        curMethod = new MethodInfo(dynamic_cast<Type *>(methodDeclaration->return_type.get()), name, methodDeclaration->first_line, methodDeclaration->first_column);

        for (auto &var : *methodDeclaration->vars) {
            var->Accept(this);
        }

        for (auto &arg : *methodDeclaration->args) {
            curVar = new VariableInfo(dynamic_cast<Type *>(arg.first.get()), dynamic_cast<Identifier *>(arg.second.get())->name,
                                      dynamic_cast<Identifier *>(arg.second.get())->first_line, dynamic_cast<Identifier *>(arg.second.get())->first_column);

            if (curMethod->args.find(curVar->symbol) != curMethod->args.end()) {
                errors.push_back("Error at line: " + std::to_string(methodDeclaration->first_line) + " column: " +
                                 std::to_string(methodDeclaration->first_column) + ". Message: argument " +
                                 dynamic_cast<Identifier *>(arg.second.get())->name->getString() +
                                 " was already declared.\n");
            }
            curMethod->args[curVar->symbol] = curVar;
        }

        if (curClass->HasMethod(curMethod->name)) {
            auto method = curClass->GetMethod(curMethod->name);
            if (method->args.size() != curMethod->args.size()) {
                errors.push_back("Error at line: " + std::to_string(methodDeclaration->first_line) +
                                 " column: " + std::to_string(methodDeclaration->first_column) + ". Message: method " +
                                 name->getString() + " was already declared.\n");
            } else {
                for (auto arg1 = method->args.begin(), arg2 = curMethod->args.begin(); arg1 != method->args.end(); arg1++, arg2++) {
                    if (arg1->second->type->type != arg2->second->type->type) {
                        errors.push_back("Error at line: " + std::to_string(methodDeclaration->first_line) +
                                         " column: " + std::to_string(methodDeclaration->first_column) +
                                         ". Message: method " + name->getString() +
                                         " already was declared.\n");
                        break;
                    }
                }
            }
        }

        curClass->methods[curMethod->name] = curMethod;

        for (auto &stat : *methodDeclaration->statements) {
            stat->Accept(this);
        }

        methodDeclaration->return_expression->Accept(this);

        if (isvalidExpr && curType != *(curMethod->type)) {
            errors.push_back("Error at line: " + std::to_string(methodDeclaration->first_line) +
                             " column: " + std::to_string(methodDeclaration->first_column) +
                             ". Message: returned type doesn't match function signature.\n");
        }

        isvalidExpr = true;
        curType = Type(NoneType{});
        curMethod = nullptr;
    }

    void visit(const Type *) override {
        std::cout << "That shouldn't happened" <<std::endl;
        assert(false);
    }

    void visit(const IfStatement *ifStatement) override {
        ifStatement->clause->Accept(this);
        if (isvalidExpr && curType != Type(BoolType{})) {
            errors.push_back("Error at line: " + std::to_string(ifStatement->first_line) + " column: " +
                             std::to_string(ifStatement->first_column) + ". Message: bool type is required.\n");
        }
        isvalidExpr = true;
        curType = Type(NoneType{});
        ifStatement->true_statement->Accept(this);
        ifStatement->false_statement->Accept(this);
    }

    void visit(const WhileStatement *whileStatement) override {
        whileStatement->clause->Accept(this);
        if (isvalidExpr && curType != Type(BoolType{})) {
            errors.push_back("Error at line: " + std::to_string(whileStatement->first_line) + " column: " +
                             std::to_string(whileStatement->first_column) + ". Message: bool type is required.\n");
        }
        isvalidExpr = true;
        curType = Type(NoneType{});
        whileStatement->body->Accept(this);
    }

    void visit(const Statement *statement) override {
        for (auto &stat : *(statement->statements)) {
            stat->Accept(this);
        }
    }

    void visit(const PrintStatement *printStatement) override {
        printStatement->print->Accept(this);
        if (isvalidExpr && curType != Type(IntType{})) {
            errors.push_back("Error at line: " + std::to_string(printStatement->first_line) + " column: " +
                             std::to_string(printStatement->first_column) + ". Message: wrong type for printing.\n");
        }
    }

    void visit(const AssignmentStatement *assignmentStatement) override {
        assignmentStatement->expr->Accept(this);
        auto name = dynamic_cast<Identifier *>(assignmentStatement->var.get())->name;

        if (curMethod->locals.find(name) != curMethod->locals.end()) {
            auto var = curMethod->locals.find(name)->second;
            if (isvalidExpr && curType != *(var->type)) {
                errors.push_back("Error at line: " + std::to_string(assignmentStatement->first_line) +
                                 " column: " + std::to_string(assignmentStatement->first_column) +
                                 ". Message: type mismatch while assigning.\n");
            }
        } else if (curClass->vars.find(name) != curClass->vars.end()) {
            auto var = curClass->vars.find(name)->second;
            if (isvalidExpr && curType != *(var->type)) {
                errors.push_back("Error at line: " + std::to_string(assignmentStatement->first_line) +
                                 " column: " + std::to_string(assignmentStatement->first_column) +
                                 ". Message: type mismatch while assigning.\n");
            }
        } else {
            errors.push_back("Error at line: " + std::to_string(assignmentStatement->first_line) +
                             " column: " + std::to_string(assignmentStatement->first_column) + ". Message: variable " +
                             name->getString() + " doesn't exist\n");
        }
        isvalidExpr = true;
        curType = Type(NoneType{});
    }

    void visit(const ArrAssignmentStatement *arrAssignmentStatement) override {
        arrAssignmentStatement->num->Accept(this);
        if (isvalidExpr && curType != Type(IntType{})) {
            errors.push_back("Error at line: " + std::to_string(arrAssignmentStatement->first_line) +
                             " column: " + std::to_string(arrAssignmentStatement->first_column) +
                             ". Message: int array type is required.\n");
        }
        isvalidExpr = true;
        curType = Type(NoneType{});

        arrAssignmentStatement->expr->Accept(this);
        if (isvalidExpr && curType != Type(IntType{})) {
            errors.push_back("Error at line: " + std::to_string(arrAssignmentStatement->first_line) + " column: " +
                             std::to_string(arrAssignmentStatement->first_column) + ". Message: int type is required.\n");
        }
        isvalidExpr = true;
        curType = Type(NoneType{});
        auto name = dynamic_cast<Identifier *>(arrAssignmentStatement->var.get())->name;
        if (curMethod->locals.find(name) == curMethod->locals.end() && curClass->vars.find(name) == curClass->vars.end()) {
            errors.push_back("Error at line: " + std::to_string(arrAssignmentStatement->first_line) +
                             " column: " + std::to_string(arrAssignmentStatement->first_column) + ". Message: variable " +
                             name->getString() + " doesn't exist\n");
        }
    }

    void visit(const AndExpression* exp) override {
        exp->expr1->Accept(this);
        if (curType != Type(BoolType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(NoneType{});
        exp->expr2->Accept(this);
        if (curType != Type(BoolType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(BoolType{});
    }

    void visit(const LessExpression* exp) override {
        exp->expr1->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(NoneType{});
        exp->expr2->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(BoolType{});
    }

    void visit(const PlusExpression* exp) override {
        exp->expr1->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(NoneType{});
        exp->expr2->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(IntType{});
    }

    void visit(const MinusExpression* exp) override {
        exp->expr1->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(NoneType{});
        exp->expr2->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(IntType{});
    }

    void visit(const MultExpression* exp) override {
        exp->expr1->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(NoneType{});
        exp->expr2->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(IntType{});
    }


    void visit(const ArrayExpression* exp) override {
        exp->expr1->Accept(this);
        if (curType != Type(IntArrType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) +
                             " column: " + std::to_string(exp->first_column) +
                             ". Message: int array type is required.\n");
        }
        curType = Type(NoneType{});
        exp->expr2->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(IntType{});
    }

    void visit(const LengthExpression* exp) override {
        exp->expr->Accept(this);
        if (curType != Type(IntArrType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) +
                             " column: " + std::to_string(exp->first_column) +
                             ". Message: int array type is required.\n");
        }
        curType = Type(IntType{});
    }

    void visit(const MethodExpression* exp) override {
        exp->class_name->Accept(this);
        auto name = dynamic_cast<Identifier *>(exp->method.get())->name;
        if (curType == Type(IntType{}) || curType == Type(NoneType{}) || curType == Type(IntArrType{}) || curType == Type(BoolType{})) {
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: calling a method " +
                             name->getString() + " from a primitive type.\n");
            isvalidExpr = false;
            return;
        }
        if (curClass->methods.find(name) == curClass->methods.end()) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) +
                             " column: " + std::to_string(exp->first_column) + ". Message: method " +
                             name->getString() + " doesn't exist.\n");
            return;
        }
        auto met = curClass->methods[name];
        if (met->args.size() != exp->params->size()) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) +
                             " column: " + std::to_string(exp->first_column) + ". Message: method " +
                             name->getString() + " was called with wrong number of args\n");
            return;
        }
        curType = *(met->type);
    }

    void visit(const Integer *) override {
        curType = Type(IntType{});
    }

    void visit(const Bool *) override {
        curType = Type(BoolType{});
    }

    void visit(const IdentExpression* exp) override {
        exp->ident->Accept(this);
    }

    void visit(const This *) override {
        curType = Type(ClassType{curClass->name});
    }

    void visit(const NewArrExpression* exp) override {
        exp->expr->Accept(this);
        if (curType != Type(IntType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) + " column: " +
                             std::to_string(exp->first_column) + ". Message: int type is required.\n");
        }
        curType = Type(IntArrType{});
    }

    void visit(const NewExpression* exp) override {
        exp->ident->Accept(this);
        if (curType == Type(IntType{}) || curType == Type(NoneType{}) || curType == Type(IntArrType{}) || curType == Type(BoolType{})) {
            isvalidExpr = false;
        }
    }

    void visit(const NotExpression* exp) override {
        exp->expr->Accept(this);
        if (curType != Type(BoolType{})) {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(exp->first_line) +
                             " column: " + std::to_string(exp->first_column) +
                             ". Message: boolean type is required.\n");
        }
        curType = Type(BoolType{});
    }

    void visit(const Expression* exp) override {
        exp->expr->Accept(this);
    }

    void visit(const Identifier *identifier) override {
        if (curClass->vars.find(identifier->name) != curClass->vars.end()) {
            auto var = curClass->vars[identifier->name];
            curType = *(var->type);
        } else if (curMethod->locals.find(identifier->name) != curMethod->locals.end()) {
            auto var = curMethod->locals[identifier->name];
            curType = *(var->type);
        } else if (curMethod->args.find(identifier->name) != curMethod->args.end()) {
            auto var = curMethod->args[identifier->name];
            curType = *(var->type);
        } else if (table->classes.find(identifier->name) != table->classes.end()) {
            auto cl = table->classes[identifier->name];
            curType = Type(ClassType{cl->name});
            curClass = cl;
        } else {
            isvalidExpr = false;
            errors.push_back("Error at line: " + std::to_string(identifier->first_line) +
                             " column: " + std::to_string(identifier->first_column) + ". Message: name " +
                             identifier->name->getString() + " wasn't declared.\n");
        }
    }

};
