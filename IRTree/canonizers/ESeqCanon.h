#pragma once

#include "../IRVisitor.h"

namespace IRT
{
    class ESeqCanon : public IRVisitor
    {

    public:
        ESeqCanon() = default;

        std::unique_ptr<const IRStatement> CanonicalTree();
        std::unique_ptr<const IRStatement> CanonicalStmTree();
        std::unique_ptr<const IRExpression> CanonicalExpTree();

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

        void updateLastExpList( std::unique_ptr<const IRExpList> newLastExpList );

        void updateLastStm( const IRStatement* newLastStm );
        void updateLastStm( std::unique_ptr<const IRStatement> newLastStm );

        std::unique_ptr<const IRExpression> canonizeExpSubtree( std::unique_ptr<const IRExpression> exp ) const;
        std::unique_ptr<const IRStatement> canonizeStmSubtree( std::unique_ptr<const IRStatement> stm ) const;

        bool areCommuting( const IRStatement* stm, const IRExpression* exp );
        const ESeqExpression* castToESeqExp( const IRExpression* exp );

        std::unique_ptr<const IRExpression> prevExp;
        std::unique_ptr<const IRStatement> prevStm;
        std::unique_ptr<const IRExpList> prevExpList;
//        std::unique_ptr<const CStmList> prevStmList;
    };

}
