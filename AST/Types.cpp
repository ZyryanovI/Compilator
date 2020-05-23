#include <iostream>
#include "Types.h"
#include "../Visitor.h"

void Type::Accept(Visitor* visitor) const {
    visitor->visit(this);
}

