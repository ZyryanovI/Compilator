#pragma once
#include <memory>

struct IRVisitor;

class IRStatement {
public:
    IRStatement() = default;
    virtual ~IRStatement() = default;

    virtual std::unique_ptr<const IRStatement> GetCopy() const = 0;

    virtual void Accept(IRVisitor* v) const = 0;
};
