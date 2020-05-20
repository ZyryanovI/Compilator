#include "Translator.h"
#include <cassert>
#include <variant>
#include <iostream>

void Translator::AddClassFields(ClassInfo *class_info) {
    if (class_info->parent != nullptr) {
        AddClassFields(class_info->parentInfo);
    }
    for (auto &var : class_info->vars) {
        curFrame->AddFormal(var.first->getString());
    }
}

void Translator::AddMethodFields(MethodInfo *method_info) {
    for (auto &arg : method_info->args) {
        curFrame->AddLocal(arg.first->getString());
    }
    for (auto &loc : method_info->locals) {
        curFrame->AddLocal(loc.first->getString());
    }
}

void Translator::BuildFrame(Symbol *class_name, Symbol *method_name) {
    ClassInfo *class_info = table->classes[class_name];
    MethodInfo *method_info = class_info->methods[method_name];

    curFrame = new X86MiniJavaFrame(class_name, method_name);

    AddClassFields(class_info);
    AddMethodFields(method_info);
}

void Translator::AcceptStms(const std::vector<std::unique_ptr<IStatement>> *statements) {
    std::unique_ptr<ISubTreeWrapper> st_list = nullptr;

    if (!statements->empty()) {
        statements->back()->Accept(this);
        st_list = std::move(curWrapper);
        for (auto stm = std::next(statements->crbegin()); stm != statements->crend(); stm++) {
            (*stm)->Accept(this);
            std::unique_ptr<ISubTreeWrapper> res = std::move(curWrapper);
            st_list = std::unique_ptr<ISubTreeWrapper>(
                new StmWrapper(new SeqStatement(res->ToStm(), st_list->ToStm())));
        }
    }

    curWrapper = std::move(st_list);
}

Translator::Translator(Table *table) : table(table) {
}

void Translator::visit(const Goal *n) {
    n->main_class->Accept(this);
    for (auto &cl : *(n->class_declarations)) {
        cl->Accept(this);
    }
}

void Translator::visit(const MainClass *n) {
    auto name = dynamic_cast<Identifier *>(n->class_name.get())->name;
    curClass = table->classes[name];
    curMethod = curClass->methods[InternTable::getIntern("main")];
    BuildFrame(curClass->name, curMethod->name);
    auto frame_name = curFrame->GetName();

    n->statement->Accept(this);
    auto wrapper = std::move(curWrapper);
    curWrapper = std::unique_ptr<ISubTreeWrapper>(
        new StmWrapper(new SeqStatement(new LabelStatement(frame_name), wrapper->ToStm())));
    CodeFragment cf(curFrame, curWrapper->ToStm());
    fragments.emplace(frame_name, std::move(cf));
    curClass = nullptr;
    curMethod = nullptr;
}

void Translator::visit(const ClassDeclaration *n) {
    auto name = dynamic_cast<Identifier *>(n->class_name.get())->name;
    curClass = table->classes[name];

    for (auto &met : *n->methods) {
        met->Accept(this);
    }

    curClass = nullptr;
}

void Translator::visit(const VarDeclaration *) {
    assert(false);
}

void Translator::visit(const MethodDeclaration *n) {
    auto name = dynamic_cast<Identifier *>(n->name.get())->name;
    curMethod = curClass->methods[name];
    BuildFrame(curClass->name, curMethod->name);

    AcceptStms(n->statements.get());
    auto stm_wrapper = std::move(curWrapper);

    n->return_expression->Accept(this);
    auto return_exp = curWrapper->ToExp();

    auto frame_name = curFrame->GetName();

    if (stm_wrapper) {
        curWrapper = std::unique_ptr<ISubTreeWrapper>(new StmWrapper(new SeqStatement(
            new LabelStatement(frame_name),
            new SeqStatement(
                stm_wrapper->ToStm(),
                new MoveStatement(curFrame->GetAccess(X86MiniJavaFrame::return_pointer)->GetExp(),
                                  return_exp)))));
    }
    CodeFragment cf(curFrame, curWrapper->ToStm());
    fragments.emplace(frame_name, std::move(cf));
    curMethod = nullptr;
}

void Translator::visit(const Type *) {
    assert(false);
}

void Translator::visit(const IfStatement *n) {
    n->clause->Accept(this);
    auto cond_wrapper = std::move(curWrapper);

    n->true_statement->Accept(this);
    auto true_wrapper = std::move(curWrapper);

    n->false_statement->Accept(this);
    auto false_wrapper = std::move(curWrapper);

    std::string true_label = "True::" + std::to_string(ifCounter);
    std::string false_label = "False::" + std::to_string(ifCounter);
    std::string exit_label = "IfExit::" + std::to_string(ifCounter);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new StmWrapper(new SeqStatement(
        cond_wrapper->ToCond(true_label, false_label),
        new SeqStatement(
            new LabelStatement(true_label),
            new SeqStatement(
                true_wrapper->ToStm(),
                new SeqStatement(
                    new JumpStatement(exit_label),
                    new SeqStatement(new LabelStatement(false_label),
                                     new SeqStatement(false_wrapper->ToStm(),
                                                      new LabelStatement(exit_label)))))))));

    ifCounter++;
}

void Translator::visit(const WhileStatement *n) {
    n->clause->Accept(this);
    auto clause_wrapper = std::move(curWrapper);

    n->body->Accept(this);
    auto body_wrapper = std::move(curWrapper);

    std::string loop_label = "Loop::" + std::to_string(whileCounter);
    std::string body_label = "Body::" + std::to_string(whileCounter);
    std::string done_label = "Done::" + std::to_string(whileCounter);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new StmWrapper(new SeqStatement(
        new LabelStatement(loop_label),
        new SeqStatement(
            clause_wrapper->ToCond(body_label, done_label),
            new SeqStatement(
                new LabelStatement(body_label),
                new SeqStatement(body_wrapper->ToStm(),
                                 new SeqStatement(new JumpStatement(loop_label),
                                                  new LabelStatement(done_label))))))));

    whileCounter++;
}

void Translator::visit(const Statement *n) {
    AcceptStms(n->statements.get());
}

void Translator::visit(const PrintStatement *n) {
    n->print->Accept(this);

    auto str = std::string("print");
    curWrapper = std::unique_ptr<ISubTreeWrapper>(
        new ExpWrapper(curFrame->ExternalCall(str, curWrapper->ToExp())));
}

void Translator::visit(const AssignmentStatement *n) {
    n->var->Accept(this);
    auto var = curWrapper->ToExp();

    n->expr->Accept(this);
    auto expr = curWrapper->ToExp();

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new StmWrapper(new MoveStatement(var, expr)));
}

void Translator::visit(const ArrAssignmentStatement *n) {
    n->var->Accept(this);
    auto var = curWrapper->ToExp();

    n->num->Accept(this);
    auto num = curWrapper->ToExp();

    n->expr->Accept(this);
    auto expr = curWrapper->ToExp();

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new StmWrapper(new MoveStatement(
        expr, new MemExpression(new BinOpExpression(
                  BinOpExpression::BinOp::PLUS, var,
                  new BinOpExpression(BinOpExpression::BinOp::MULT,
                                      new BinOpExpression(BinOpExpression::BinOp::PLUS, num,
                                                          new ConstExpression(1)),
                                      new ConstExpression(curFrame->WordSize())))))));
}

void Translator::visit(const AndExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(
        new BinOpExpression(BinOpExpression::BinOp::AND, expr1->ToExp(), expr2->ToExp())));
}

void Translator::visit(const LessExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(
        new CondWrapper(CJumpStatement::Relation::LESS, expr1->ToExp(), expr2->ToExp()));
}

void Translator::visit(const PlusExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(
        new BinOpExpression(BinOpExpression::BinOp::PLUS, expr1->ToExp(), expr2->ToExp())));
}

void Translator::visit(const MinusExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(
        new BinOpExpression(BinOpExpression::BinOp::MINUS, expr1->ToExp(), expr2->ToExp())));
}

void Translator::visit(const MultExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(
        new BinOpExpression(BinOpExpression::BinOp::MULT, expr1->ToExp(), expr2->ToExp())));
}

void Translator::visit(const OrExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(
        new BinOpExpression(BinOpExpression::BinOp::OR, expr1->ToExp(), expr2->ToExp())));
}

void Translator::visit(const RemainExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(
        new BinOpExpression(BinOpExpression::BinOp::REM, expr1->ToExp(), expr2->ToExp())));
}

void Translator::visit(const ArrayExpression *n) {
    n->expr1->Accept(this);
    auto expr1 = std::move(curWrapper);

    n->expr2->Accept(this);
    auto expr2 = std::move(curWrapper);

    curWrapper =
        std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(new MemExpression(new BinOpExpression(
            BinOpExpression::BinOp::PLUS, expr1->ToExp(),
            new BinOpExpression(BinOpExpression::BinOp::MULT,
                                new BinOpExpression(BinOpExpression::BinOp::PLUS, expr2->ToExp(),
                                                    new ConstExpression(1)),
                                new ConstExpression(curFrame->WordSize()))))));
}

void Translator::visit(const LengthExpression *n) {
    n->expr->Accept(this);
    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(curWrapper->ToExp()));
}

void Translator::visit(const MethodExpression *n) {
    // NOT DONE YET
    n->class_name->Accept(this);
    std::unique_ptr<ISubTreeWrapper> class_name = std::move(curWrapper);

    auto arg_list = new IRExpList(class_name->ToExp());
    for (auto &arg : *n->params) {
        arg->Accept(this);
        arg_list->Add(curWrapper->ToExp());
    }

    auto method_name = callerClass->getString() +
                       "::" + dynamic_cast<Identifier *>(n->method.get())->name->getString();

    curWrapper = std::unique_ptr<ISubTreeWrapper>(
        new ExpWrapper(new CallExpression(new NameExpression(method_name), arg_list)));
}

void Translator::visit(const Integer *n) {
    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(new ConstExpression(n->num)));
}

void Translator::visit(const Bool *n) {
    curWrapper =
        std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(new ConstExpression(n->b ? 1 : 0)));
}

void Translator::visit(const IdentExpression *n) {
    n->ident->Accept(this);
}

void Translator::visit(const This *) {
    curWrapper = std::unique_ptr<ISubTreeWrapper>(
        new ExpWrapper(curFrame->GetAccess(X86MiniJavaFrame::this_pointer)->GetExp()));
    callerClass = curClass->name;
}

void Translator::visit(const NewArrExpression *n) {
    n->expr->Accept(this);
    auto expr = curWrapper->ToExp();

    auto str = std::string("malloc");

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(curFrame->ExternalCall(
        str, new BinOpExpression(
                 BinOpExpression::BinOp::MULT,
                 new BinOpExpression(BinOpExpression::BinOp::PLUS, expr, new ConstExpression(1)),
                 new ConstExpression(curFrame->WordSize())))));
}

void Translator::visit(const NewExpression *n) {
    auto classInfo = table->classes[dynamic_cast<Identifier *>(n->ident.get())->name];
    int fields = classInfo->GetSize();

    auto str = std::string("malloc");

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(curFrame->ExternalCall(
        str, new BinOpExpression(BinOpExpression::BinOp::MULT, new ConstExpression(fields),
                                 new ConstExpression(curFrame->WordSize())))));

    callerClass = dynamic_cast<Identifier *>(n->ident.get())->name;
}

void Translator::visit(const NotExpression *n) {
    n->expr->Accept(this);

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new CondNotWrapper(curWrapper.release()));
}

void Translator::visit(const Expression *n) {
    n->expr->Accept(this);
}

void Translator::visit(const Identifier *n) {
    // std::cout << n->name->getString() << "\n";
    // assert(false);

    // auto name = dynamic_cast<Identifier*>(n->ident.get())->name
    auto name = n->name;
    IAccess *address = curFrame->GetAccess(name->getString());
    Type *type;

    curWrapper = std::unique_ptr<ISubTreeWrapper>(new ExpWrapper(address->GetExp()));
    auto local = curMethod->locals.find(name);
    auto arg = curMethod->args.find(name);
    if (local == curMethod->locals.end() && arg == curMethod->args.end()) {
        type = curClass->vars[name]->type;
    } else if (local != curMethod->locals.end()) {
        type = local->second->type;
    } else {
        type = arg->second->type;
    }

    if (auto classVal = std::get_if<ClassType>(&(type->type))) {
        callerClass = classVal->name;
    }
}
