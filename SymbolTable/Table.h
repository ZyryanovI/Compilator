#pragma once
#include "ClassInfo.h"
#include "Symbol.h"
#include <unordered_map>

class Table {
public:
    std::unordered_map<Symbol*, ClassInfo*> classes;
};
