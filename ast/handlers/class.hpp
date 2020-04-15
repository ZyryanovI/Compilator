#pragma once

#include <string>
#include <vector>
#include <memory>

#include <yyltype.hpp>
#include <handlers/visitable.hpp>
#include <visitor.hpp>

#include <handlers/var_declaration.hpp>
#include <handlers/statements.hpp>
#include <handlers/method_declaration.hpp>

namespace ast {

class Class : public IVisitable {
public:

    Class(const std::string &name,
          const std::string &parent,
          const std::vector<PVarDeclaration>& var_declarations,
          const std::vector<PMethodDeclaration>& methods,
            MiniJava::YYLTYPE pos) : identifier_(name), parent_(parent),
                              var_declarations_(var_declarations), method_declarations_(methods) { setPos(pos); }


    Class(const std::string &name,
                const std::vector<PVarDeclaration>& var_declarations,
                const std::vector<PMethodDeclaration>& methods,
            MiniJava::YYLTYPE pos) : identifier_(name), var_declarations_(var_declarations), method_declarations_(methods) { setPos(pos); }

    const std::string& getParent() const {
        return parent_;
    }
    const std::string& getIdentifier() const {
        return identifier_;
    }

    const std::vector<PVarDeclaration>& getVarDeclaration() const {
        return var_declarations_;
    }

    const std::vector<PMethodDeclaration>& getMethodDeclaration() const {
        return method_declarations_;
    }

    void accept(IVisitor *visitor) const { visitor->visit(this); }

private:
    std::string parent_ = "";
    std::string identifier_;
    std::vector<PVarDeclaration> var_declarations_;
    std::vector<PMethodDeclaration> method_declarations_;

    void setPos(const MiniJava::YYLTYPE pos) { pos_ = pos; }
};

typedef std::shared_ptr<Class> PClass;
}
