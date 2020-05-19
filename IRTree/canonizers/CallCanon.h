#pragma once

#include "../IRVisitor.h"


namespace IRT
{
    class CallCanon : public IRVisitor
    {

    public:
        CallCanon() = default;

        std::unique_ptr<const IRStatement> CanonicalTree();

        void visit( const ConstExpression* n ) override;
        void visit( const NameExpression* n ) override;
        void visit( const TempExpression* n ) override;
        void visit( const BinOpExpression* n ) override;
        void visit( const MemExpression* n ) override;
        void visit( const CallExpression* n ) override;

        void visit( const ESeqExpression* n ) override;

        void visit( const MoveStatement* n ) override;  //+
        void visit( const ExpStatement* n ) override;  //+
        void visit( const JumpStatement* n ) override;  //+
        void visit( const CJumpStatement* n ) override;  //+
        void visit( const SeqStatement* n ) override;  //+
        void visit( const LabelStatement* n ) override;  //+

        void visit( const IRExpList* list ) override;  //+
        //void visit( const CStmList* list ) override;

    private:
        void updateLastExp( const IRExpression* newLastExp );
        void updateLastExp( std::unique_ptr<const IRExpression> newLastExp );

        void updateLastExpList( const IRExpList* newLastExpList );
        void updateLastExpList( std::unique_ptr<IRExpList> newLastExpList );

        void updateLastStm( const IRStatement* newLastStm );
        void updateLastStm( std::unique_ptr<const IRStatement> newLastStm );

//        void updateLastStmList( const CStmList* newLastStmList );
//        void updateLastStmList( std::unique_ptr<const CStmList> newLastStmList );

        std::unique_ptr<const IRExpList> prevExpList;
        std::unique_ptr<const IRExpression> prevExp;
        std::unique_ptr<const IRStatement> prevStm;
//        std::unique_ptr<const CStmList> prevStmList;
    };

}
