#include "IAccess.h"
#include "../IRTree/Expression.h"
#include <string>

class InRegAccess : public IAccess {
public:
    InRegAccess(std::string name);
    IRExpression* GetExp() override;

private:
    std::string name;
};
