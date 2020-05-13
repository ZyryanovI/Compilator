#include "IFrame.h"
#include "InRegAccess.h"
#include "InFrameAccess.h"
#include "../SymbolTable/Symbol.h"
#include <string>
#include <map>

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
    // int FormalsCount() override;
    // IAccess* Formal(int index) override;
    // IAccess* FindLocalOrFormal(const Symbol* name) override;
private:
    std::string name;
    int frame_size;
    const int word_size = 4;
    std::map<std::string, IAccess*> addresses;
};
