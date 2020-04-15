#pragma once

#include <string>
#include <memory>

#include <yyltype.hpp>
#include <handlers/visitable.hpp>
#include <visitor.hpp>

#include <handlers/types.hpp>

namespace ast {

class VarDeclaration : public IVisitable {
public:
    VarDeclaration(const PType &type, const std::string &identifier, MiniJava::YYLTYPE pos) : type_(type), identifier_(identifier) { setPos(pos); }

    const PType &getType() const {
        return type_;
    }

    const std::string& getIdentifier() const {
        return identifier_;
    }

    void accept(IVisitor *visitor) const { visitor->visit(this); }
private:
    std::string identifier_;
    PType type_;

    void setPos(const MiniJava::YYLTYPE pos) { pos_ = pos; }
};

typedef std::shared_ptr<VarDeclaration> PVarDecalartion;
}
