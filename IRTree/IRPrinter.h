#include "IRVisitor.h"

class IRPrinter : public IRVisitor {
public:
    IRPrinter(std::string filename);
    ~IRPrinter() override;

    void visit(const ConstExpression* n) override;
    void visit(const NameExpression* n) override;
    void visit(const TempExpression* n) override;
    void visit(const BinOpExpression* n) override;
    void visit(const MemExpression* n) override;
    void visit(const CallExpression* n) override;
    void visit(const ESeqExpression* n) override;

    void visit(const MoveStatement* n) override;
    void visit(const ExpStatement* n) override;
    void visit(const JumpStatement* n) override;
    void visit(const CJumpStatement* n) override;
    void visit(const SeqStatement* n) override;
    void visit(const LabelStatement* n) override;

    void visit( const IRExpList* n) override;

private:
    FILE* f;
    int nodeNumber;

    std::string getRel(CJumpStatement::Relation rel);
    std::string getBinop(BinOpExpression::BinOp rel);
};
