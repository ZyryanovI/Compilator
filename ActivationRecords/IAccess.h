#pragma once
#include "../IRTree/Expression.h"

class IAccess {
public:
    virtual ~IAccess() = default;
    virtual IRExpression* GetExp() = 0;
};
