#pragma once
#include <string>
#include <unordered_map>
#include <list>

class Symbol {
public:
    Symbol(const std::string& str_);

    const std::string& getString() const;

    Symbol() = delete;
    Symbol(const Symbol&) = delete;
    void operator=(const Symbol&) = delete;

private:
    const std::string& str;
};

class InternTable {
public:
    static Symbol* getIntern(const std::string& str);

private:
    static std::unordered_map<std::string, Symbol*> table;
    static std::list<std::string> allStrs;
};
