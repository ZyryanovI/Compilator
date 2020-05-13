#include "IAccess.h"

class InFrameAccess : public IAccess {
public:
    InFrameAccess(IAccess* address, int offset);
    IRExpression* GetExp() override;

private:
    IAccess* address;
    int offset;
};
