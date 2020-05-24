#pragma once

#include "Interfaces.hpp"
#include "Identifier.hpp"

#include <variant>

//переопределим операторы для типов(так будет удобнее потом)
struct NoneType {
    bool operator==(const NoneType&) const {
        return true;
    }
    bool operator!=(const NoneType&) const {
        return false;
    }
};

struct IntType {
    bool operator==(const IntType&) const {
        return true;
    }
    bool operator!=(const IntType&) const {
        return false;
    }
};

struct BoolType {
    bool operator==(const BoolType&) const {
        return true;
    }
    bool operator!=(const BoolType&) const {
        return false;
    }
};

struct IntArrType {
    bool operator==(const IntArrType&) const {
        return true;
    }
    bool operator!=(const IntArrType&) const {
        return false;
    }
};

struct ClassType {
    Symbol* name;

    bool operator==(const ClassType& t) const {
        return name == t.name;
    }
    bool operator!=(const ClassType& t) const {
        return name != t.name;
    }
};

using TYPE = std::variant<NoneType, IntType, BoolType, IntArrType, ClassType>; //типы

class Type : public IType {
public:
    TYPE type;

    bool operator==(const Type& t) const {
        return type == t.type;
    }
    bool operator!=(const Type& t) const {
        return type != t.type;
    }

    template <typename T>
    Type(T&& t) : type(t) {}

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};
