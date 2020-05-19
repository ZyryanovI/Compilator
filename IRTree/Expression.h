// #include "../SymbolTable/Symbol.h"
#pragma once
// #ifndef IRTree_Expression
// #define IRTree_Expression

#include "IRExpression.h"
#include "IRStatement.h"
#include <vector>
#include <string>
#include <memory>
#include "Label.h"


class IRExpList {
public:
    IRExpList() = default;
//    IRExpList( std::unique_ptr<const IRExpression> exp);

    void Accept(IRVisitor* v) const;

    void Add( IRExpression* exp);

    std::unique_ptr<const IRExpList> GetCopy() const;


    explicit IRExpList( const IRExpression* expression )
    {
        Add( expression );
    }

    void Add( const IRExpression* expression )
    {
        list.emplace_back( expression );
    }

    void Add( std::unique_ptr<const IRExpression> expression )
    {
        list.emplace_back( std::move( expression ) );
    }
//
//

    const std::vector<std::unique_ptr<const IRExpression>>& GetExpressions() const
    {
        return list;
    }

    std::vector< std::unique_ptr<const IRExpression> > list;

    bool is_commutative() {return false; }
    bool is_absolutely_commutative() {return false; }
};

class ConstExpression : public IRExpression {  //done
public:
    ConstExpression(int val);
    void Accept(IRVisitor* v) const override;

    int GetValue() const { return value; }

    std::unique_ptr<const IRExpression> GetCopy() const override;


    bool is_commutative() {return true; }
    bool is_absolutely_commutative() {return true; }

    const int value;
};

class NameExpression : public IRExpression {    //done   // после 3 конец, шаг 5. (деструктор)
public:
    explicit  NameExpression(const Label& name) : name(name) {}
    explicit  NameExpression(const std::string& name) : name(name) {}
    void Accept(IRVisitor* v) const override;

    const Label&  GetLabel() const { return name; }

//    std::string name;
    Label name;

    std::unique_ptr<const IRExpression> GetCopy() const override;

    bool is_commutative() {return true; }
    bool is_absolutely_commutative() {return true; }
};



class TempExpression : public IRExpression {        // после 3конец шаг 3 (деструктор, как и раньше)
public:
    explicit  TempExpression(const std::string& name) : name(name) {}
    explicit  TempExpression(const Temp& name) : name(name) {}
    void Accept(IRVisitor* v) const override;
    const Temp GetTemp() const { return name; }
    Temp name;


    std::unique_ptr<const IRExpression> GetCopy() const override;

    bool is_commutative() {return true; }
    bool is_absolutely_commutative() {return false; }   // 123 mb bug
};

class BinOpExpression : public IRExpression {   //8+ шаг, 2 раза подр
public:
    enum BinOp { PLUS, MINUS, MULT, DIV, REM, LESS, AND, OR };

    BinOpExpression(BinOpExpression::BinOp binop, const IRExpression* left, const IRExpression* right);

    BinOpExpression( BinOpExpression::BinOp binop, std::unique_ptr<const IRExpression> left, std::unique_ptr<const IRExpression> right ) :
            binop( binop ), left( std::move( left ) ), right( std::move( right ) )
    {
    }

    const IRExpression* GetLeft() const { return left.get(); }
    const IRExpression* GetRight() const { return right.get(); }
    const BinOp GetType() const { return binop; }


    void Accept(IRVisitor* v) const override;

    std::unique_ptr<const IRExpression> GetCopy() const override;

    bool is_commutative() {return false; }
    bool is_absolutely_commutative() {return false; }


    const BinOpExpression::BinOp binop;
    std::unique_ptr<const IRExpression> left;
    std::unique_ptr<const IRExpression> right;
};

class MemExpression : public IRExpression {
public:
    explicit MemExpression( std::unique_ptr<const IRExpression> exp_ ) :
            expr( std::move( exp_ ) )
    {}

    explicit MemExpression( const IRExpression* expr);

    const IRExpression* GetMem() const { return expr.get(); }

    void Accept(IRVisitor* v) const override;
    std::unique_ptr<const IRExpression> expr;

    std::unique_ptr<const IRExpression> GetCopy() const override;
    //mb тут не нужен explicit



//    bool is_commutative() {return expr.is_absolutely_commutative(); }
//    bool is_absolutely_commutative() {return false; }
};

class CallExpression : public IRExpression {    // после 3end, потом тут(деструктор)
public:
    CallExpression(IRExpression* func, IRExpList* args);
    void Accept(IRVisitor* v) const override;


    const IRExpression* GetFuncExp() const { return func.get(); }
    const IRExpList* GetArgs() const { return args.get(); }


    std::unique_ptr<const IRExpression> GetCopy() const override;

    CallExpression( std::unique_ptr<const IRExpression> funcExp, std::unique_ptr<const IRExpList> args ) :
            func( std::move( funcExp ) ), args( std::move( args ) )
    {
    }


    bool is_commutative() {return false; }
    bool is_absolutely_commutative() {return false; }

    std::unique_ptr<const IRExpression> func;
    std::unique_ptr<const IRExpList> args;
};

class ESeqExpression : public IRExpression {    //после 3end, вначале тут
public:
    ESeqExpression(const IRStatement* stm, const IRExpression* expr);

    ESeqExpression( std::unique_ptr<const IRStatement> stm, std::unique_ptr<const IRExpression> exp ) :
    stm( std::move( stm ) ), expr( std::move( exp ) )
    {
    }
    void Accept(IRVisitor* v) const override;
    const IRStatement* GetStm() const { return stm.get();}
    const IRExpression* GetExp() const { return expr.get();}

    std::unique_ptr<const IRExpression> GetCopy() const override;


    bool is_commutative() {return false; }
    bool is_absolutely_commutative() {return false; }

    std::unique_ptr<const IRStatement> stm;
    std::unique_ptr<const IRExpression> expr;

    // 123 mb bug
//    bool is_commutative() {return false; }
//    bool is_absolutely_commutative() {return false; }
};

// #endif  // IRTree_Expression
