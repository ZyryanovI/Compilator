#pragma once
#include "../SymbolTable/Symbol.h"
#include "../IRTree/IRExpression.h"
#include "IAccess.h"
#include <string>

class IFrame {
public:
    virtual ~IFrame() = default;
    virtual void AddFormal(const std::string& name) = 0;
    virtual void AddLocal(const std::string& name) = 0;
    virtual std::string GetName() = 0;
    virtual IRExpression* ExternalCall(std::string& method_name, IRExpression* args) = 0;
    virtual IAccess* GetAccess(const std::string& access_name) = 0;
    virtual int WordSize() = 0;
    // virtual int FormalsCount() const = 0;
    // virtual IAccess* Formal(int index) const = 0;
    // virtual IAccess* FindLocalOrFormal(std::string& name) const = 0;
};
