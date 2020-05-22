#pragma once

#include "AST/Goal.h"

//дополнить для остальных классов, когда напишу их
class Visitor{
public:
    virtual void visit(const Goal*) = 0;
};

