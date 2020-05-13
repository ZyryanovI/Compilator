#include "IRStatement.h"
#include "../ActivationRecords/IFrame.h"

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
