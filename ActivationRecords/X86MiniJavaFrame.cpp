#include "X86MiniJavaFrame.h"

const std::string X86MiniJavaFrame::frame_pointer = "__frame__";
const std::string X86MiniJavaFrame::this_pointer = "__this__";
const std::string X86MiniJavaFrame::return_pointer = "__return__";

X86MiniJavaFrame::X86MiniJavaFrame(Symbol *class_name, Symbol *method_name)
    : name(class_name->getString() + "::" + method_name->getString()), frame_size(0) {
    addresses[frame_pointer] = new InRegAccess(frame_pointer);
    frame_size += word_size;
    addresses[this_pointer] = new InRegAccess(this_pointer);
    frame_size += word_size;
    addresses[return_pointer] = new InRegAccess(return_pointer);
    frame_size += word_size;
}

void X86MiniJavaFrame::AddLocal(const std::string &local_name) {
    addresses[local_name] = new InFrameAccess(GetAccess(frame_pointer), frame_size);
    frame_size += word_size;
}

void X86MiniJavaFrame::AddFormal(const std::string &formal_name) {
    addresses[formal_name] = new InFrameAccess(GetAccess(this_pointer), frame_size);
    frame_size += word_size;
}

IAccess *X86MiniJavaFrame::GetAccess(const std::string &access_name) {
    auto res = addresses.find(access_name);
    if (res != addresses.end()) {
        return res->second;
    } else {
        return nullptr;
    }
}

IRExpression *X86MiniJavaFrame::ExternalCall(std::string &method_name, IRExpression *args) {
    return new CallExpression(new NameExpression(method_name), new IRExpList(args));
}
