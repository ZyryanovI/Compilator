#pragma once
#include <memory>


struct IRVisitor;

class IRExpression {
public:
    IRExpression() = default;
    virtual ~IRExpression() = default;      // после 3 конец, 4,6,... шаг. (деструктор)

    virtual std::unique_ptr<const IRExpression> GetCopy() const = 0;

    virtual void Accept(IRVisitor* v) const = 0;
};
