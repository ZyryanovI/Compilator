#pragma once
#include "Interfaces.h"
#include <string>

class Identifier : public IIdentifier {
public:
    std::string name;

    Identifier(int first_line, int first_column, std::string name);

    void Accept(Visitor* visitor) const;

};