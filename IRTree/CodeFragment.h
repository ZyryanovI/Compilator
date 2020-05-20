#pragma once
#include "../SymbolTable/Symbol.h"
#include "IRExpression.h"
#include "IRStatement.h"
#include "Expression.h"
#include <string>
#include <map>

class IAccess {
public:
    virtual ~IAccess() = default;
    virtual IRExpression* GetExp() = 0;
};

class InFrameAccess : public IAccess {
public:
    InFrameAccess(IAccess* address, int offset);
    IRExpression* GetExp() override;

private:
    IAccess* address;
    int offset;
};

class InRegAccess : public IAccess {
public:
    InRegAccess(std::string name);
    IRExpression* GetExp() override;

private:
    std::string name;
};

class IFrame {
public:
    virtual ~IFrame() = default;
    virtual void AddFormal(const std::string& name) = 0;
    virtual void AddLocal(const std::string& name) = 0;
    virtual std::string GetName() = 0;
    virtual IRExpression* ExternalCall(std::string& method_name, IRExpression* args) = 0;
    virtual IAccess* GetAccess(const std::string& access_name) = 0;
    virtual int WordSize() = 0;
};

class X86MiniJavaFrame : public IFrame {
public:
    X86MiniJavaFrame(Symbol* class_name, Symbol* method_name);
    std::string GetName() override {
        return name;
    }
    void AddFormal(const std::string& formal_name) override;
    void AddLocal(const std::string& local_name) override;
    IAccess* GetAccess(const std::string& access_name) override;
    IRExpression* ExternalCall(std::string& method_name, IRExpression* args) override;
    int WordSize() override {
        return word_size;
    }

    const static std::string frame_pointer;
    const static std::string this_pointer;
    const static std::string return_pointer;
private:
    std::string name;
    int frame_size;
    const int word_size = 4;
    std::map<std::string, IAccess*> addresses;
};

class CodeFragment {
public:
    CodeFragment(IFrame* frame, IRStatement* body) : frame(frame), body(body), rootCanonIRT( nullptr ) {
    }

    CodeFragment(CodeFragment&& other)
        : frame(std::move(other.frame)), body(std::move(other.body)), rootCanonIRT( nullptr ) {
    }

    IFrame* frame;
    IRStatement* body;
    std::unique_ptr<const IRStatement> rootCanonIRT;
};
