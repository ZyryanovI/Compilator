#include "InFrameAccess.h"

InFrameAccess::InFrameAccess(IAccess *address, int offset) : address(address), offset(offset) {
}

IRExpression *InFrameAccess::GetExp() {
    IRExpression *return_exp;
    if (offset != 0) {
        return_exp = new BinOpExpression(BinOpExpression::BinOp::PLUS, address->GetExp(),
                                         new ConstExpression(offset));
    } else {
        return_exp = address->GetExp();
    }
    return new MemExpression(return_exp);
}
