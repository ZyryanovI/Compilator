#pragma once

#include <string>
#include <list>
#include <unordered_map>


class Symbol {
public:
    Symbol(const std::string& str_);

    Symbol() = delete;
    Symbol(const Symbol&) = delete;
    void operator=(const Symbol&) = delete;

    const std::string& getString() const;

private:
    const std::string& str;
};

class InternTable {
public:
    static Symbol* getIntern(const std::string& str);

private:
    static std::list<std::string> allStrs;
    static std::unordered_map<std::string, Symbol*> table;
};
