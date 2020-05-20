#include "Type.h"
#include "../Visitor.h"
#include <iostream>

void Type::Accept(Visitor *v) const {
    v->visit(this);
}
