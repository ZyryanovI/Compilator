#pragma once

#include <string>
#include <list>
#include <unordered_map>


class Symbol {
public:
    Symbol(const std::string &str_) : str(str_) {}

    Symbol() = delete;
    Symbol(const Symbol&) = delete;
    void operator=(const Symbol&) = delete;

    const std::string& getString() const {
	    return str;
	}

private:
    const std::string& str;
};

class InternTable {
public:
    static Symbol* getIntern(const std::string &str) {
	    auto find_str = table.find(str);
	    if (find_str != table.end()) {
	        return find_str->second;
	    }
	    allStrs.push_back(str);
	    auto val = new Symbol(allStrs.back());
	    table.insert({allStrs.back(), val});
	    return val;
	}

private:
    static std::list<std::string> allStrs;
    static std::unordered_map<std::string, Symbol*> table;
};
