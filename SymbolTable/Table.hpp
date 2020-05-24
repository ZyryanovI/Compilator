#pragma once

#include "Symbol.hpp"
#include "ClassInfo.hpp"
#include <unordered_map>

class Table {
public:
    std::unordered_map<Symbol*, ClassInfo*> classes;
};
