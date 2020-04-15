#pragma once

#include <iostream>
#include <string>
#include <memory>

namespace ast {

class ExpressionInt;
class ExpressionBinaryOp;
class ExpressionLogical;
class ExpressionId;
class ExpressionSquareBracket;
class ExpressionLen;
class ExpressionUnaryNegation;
class ExpressionThis;
class TypeInt;
class TypeBoolean;
class TypeArray;
class TypeClass;
class StatementAssign;
class MainClass;

typedef std::shared_ptr<ExpressionInt> PExpressionInt;
typedef std::shared_ptr<ExpressionBinaryOp> PExpressionBinaryOp;
typedef std::shared_ptr<ExpressionLogical> PExpressionLogical;
typedef std::shared_ptr<ExpressionId> PExpressionId;
typedef std::shared_ptr<ExpressionSquareBracket> PExpressionSquareBracket;
typedef std::shared_ptr<ExpressionLen> PExpressionLen;
typedef std::shared_ptr<ExpressionUnaryNegation> PExpressionUnaryNegation;
typedef std::shared_ptr<ExpressionThis> PExpressionThis;
typedef std::shared_ptr<TypeInt> PTypeInt;
typedef std::shared_ptr<TypeBoolean> PTypeBoolean;
typedef std::shared_ptr<TypeArray> PTypeArray;
typedef std::shared_ptr<TypeClass> PTypeClass;
typedef std::shared_ptr<StatementAssign> PStatementAssign;
typedef std::shared_ptr<MainClass> PMainClass;

class IVisitor {
public:
    virtual ~IVisitor() = default;

    virtual void visit(const ExpressionInt* expr) = 0;
    virtual void visit(const ExpressionBinaryOp* expr) = 0;
    virtual void visit(const ExpressionLogical* expr) = 0;
    virtual void visit(const ExpressionId* expr) = 0;
    virtual void visit(const ExpressionSquareBracket* expr) = 0;
    virtual void visit(const ExpressionLen* expr) = 0;
    virtual void visit(const ExpressionUnaryNegation* expr) = 0;
    virtual void visit(const ExpressionThis* expr) = 0;
    virtual void visit(const TypeInt* type) = 0;
    virtual void visit(const TypeBoolean* type) = 0;
    virtual void visit(const TypeArray* type) = 0;
    virtual void visit(const TypeClass* type) = 0;
    virtual void visit(const StatementAssign* statement) = 0;
    virtual void visit(const MainClass* main_class) = 0;
};

}

