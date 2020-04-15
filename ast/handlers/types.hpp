#pragma once

#include <string>
#include <memory>

#include <yyltype.hpp>
#include <handlers/visitable.hpp>
#include <visitor.hpp>

#include <handlers/expressions.hpp>

namespace ast {

/***************************************************************************************************/

class Type : public IVisitable {
public:
    Type() = default;
    virtual std::string getType() const {return "";};
protected:
    void setPos(const MiniJava::YYLTYPE pos) { pos_ = pos; }
};

typedef std::shared_ptr<Type> PType;

/***************************************************************************************************/

class TypeInt : public Type {
public:
    TypeInt(MiniJava::YYLTYPE pos) { setPos(pos); }

    std::string getType() const { return "INT"; }
    void accept(IVisitor *visitor) const { visitor->visit(this); }
};

typedef std::shared_ptr<TypeInt> PTypeInt;

class TypeBoolean : public Type {
public:
    TypeBoolean(MiniJava::YYLTYPE pos) { setPos(pos); }

    std::string getType() const { return "Boolean"; }
    void accept(IVisitor *visitor) const { visitor->visit(this); }
};

typedef std::shared_ptr<TypeBoolean> PTypeBoolean;


class TypeArray : public Type {
public:
    TypeArray(MiniJava::YYLTYPE pos) { setPos(pos); }

    std::string getType() const { return "Array"; }
    void accept(IVisitor *visitor) const { visitor->visit(this); }
};

typedef std::shared_ptr<TypeArray> PTypeArray;

class TypeClass : public Type {
public:
    TypeClass(const std::string& name, MiniJava::YYLTYPE pos): name_(name) { setPos(pos); }

    std::string getType() const { return name_; }
    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    std::string name_;
};

typedef std::shared_ptr<TypeClass> PTypeClass;


}

