#include "Symbol.h"

Symbol::Symbol(const std::string &str_) : str(str_) {
}

Symbol *InternTable::getIntern(const std::string &str) {
    auto find_str = table.find(str);
    if (find_str != table.end()) {
        return find_str->second;
    }
    allStrs.push_back(str);
    auto val = new Symbol(allStrs.back());
    table.insert({allStrs.back(), val});
    return val;
}

const std::string &Symbol::getString() const {
    return str;
}

std::unordered_map<std::string, Symbol *> InternTable::table;
std::list<std::string> InternTable::allStrs;
