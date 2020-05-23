#pragma once

#include "Symbol.h"
#include "ClassInfo.h"
#include <unordered_map>

class Table {
public:
    std::unordered_map<Symbol*, ClassInfo*> classes;
};
