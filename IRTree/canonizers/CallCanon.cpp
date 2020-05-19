#include <memory>

#include "CallCanon.h"


namespace IRT
{

    std::unique_ptr<const IRStatement> CallCanon::CanonicalTree()
    {
        return std::move( prevStm );
    }

    void CallCanon::updateLastExp( const IRExpression* newLastExp )
    {
        prevExp = std::move( std::unique_ptr<const IRExpression>( newLastExp ) );
    }

    void CallCanon::updateLastExp( std::unique_ptr<const IRExpression> newLastExp )
    {
        prevExp = std::move( newLastExp );
    }

    void CallCanon::updateLastExpList( const IRExpList* newLastExpList )
    {
        prevExpList = std::move( std::unique_ptr<const IRExpList>( newLastExpList ) );
    }

    void CallCanon::updateLastExpList( std::unique_ptr<IRExpList> newLastExpList )
    {
        prevExpList = std::move( newLastExpList );
    }

    void CallCanon::updateLastStm( const IRStatement* newLastStm )
    {
        prevStm = std::move( std::unique_ptr<const IRStatement>( newLastStm ) );
    }

    void CallCanon::updateLastStm( std::unique_ptr<const IRStatement> newLastStm )
    {
        prevStm = std::move( newLastStm );
    }

//    void CallCanon::updateLastStmList( const CStmList* newLastStmList )
//    {
//        prevStmList = std::move( std::unique_ptr<const CStmList>( newLastStmList ) );
//    }
//
//    void CallCanon::updateLastStmList( std::unique_ptr<const CStmList> newLastStmList )
//    {
//        prevStmList = std::move( newLastStmList );
//    }

    void CallCanon::visit( const ConstExpression* n )
    {
        updateLastExp( std::make_unique<const ConstExpression>( n->value ) );
    }

    void CallCanon::visit( const NameExpression* n )
    {
        updateLastExp( std::make_unique<const NameExpression>( n->name ) );
    }

    void CallCanon::visit( const TempExpression* n )
    {
        updateLastExp( std::make_unique<const TempExpression>( n->name ) );
    }

    void CallCanon::visit( const BinOpExpression* n )
    {
        n->left->Accept( this );
        std::unique_ptr<const IRExpression> nLeft = std::move( prevExp );

        n->right->Accept( this );
        std::unique_ptr<const IRExpression> nRight = std::move( prevExp );

        updateLastExp(
                std::make_unique<const BinOpExpression>(
                        n->binop,
                        nLeft.release(),
                        nRight.release()
                )
        );
    }

    void CallCanon::visit( const MemExpression* n )
    {
        n->expr->Accept( this );
        std::unique_ptr<const IRExpression> addressExp = std::move( prevExp );

        updateLastExp(
                std::make_unique<const MemExpression>( addressExp.release() )
        );
    }

    void CallCanon::visit( const CallExpression* n )
    {
        n->func->Accept( this );
        std::unique_ptr<const IRExpression> functionExp = std::move( prevExp );

        n->args->Accept(this);
        std::unique_ptr<const IRExpList> argumentsList = std::move( prevExpList );

        Temp temp;
        updateLastExp(
                std::make_unique<const ESeqExpression>(
                        std::make_unique<const MoveStatement>(
                                std::make_unique<const TempExpression>( temp ),
                                std::make_unique<const CallExpression>(
                                        std::move( functionExp ),
                                        std::move( argumentsList )
                                )
                        ),
                        std::make_unique<const TempExpression>( temp )
                ) );
    }

    void CallCanon::visit( const ESeqExpression* n )
    {
        n->stm->Accept( this );
        n->expr->Accept( this );

        updateLastExp(
                std::make_unique<const ESeqExpression>(
                        prevStm.get(),
                        prevExp.get()
                )
        );
    }

    void CallCanon::visit( const ExpStatement* n )
    {
        n->exp->Accept( this );
        std::unique_ptr<const IRExpression> exp = std::move( prevExp );

        updateLastStm(
                std::make_unique<const ExpStatement>( exp.release() )
        );
    }

    void CallCanon::visit( const CJumpStatement* n )
    {
        n->left->Accept( this );
        std::unique_ptr<const IRExpression> nLeft = std::move( prevExp );

        n->right->Accept( this );
        std::unique_ptr<const IRExpression> nRight = std::move( prevExp );

        updateLastStm(
                std::make_unique<const CJumpStatement>(
                        n->rel,
                        nLeft.release(),
                        nRight.release(),
                        n->if_left,
                        n->if_right
                )
        );
    }

    void CallCanon::visit( const JumpStatement* n )
    {
        updateLastStm(
                std::make_unique<const JumpStatement>( n->label )
        );
    }

    void CallCanon::visit( const LabelStatement* n )
    {
        updateLastStm(
                std::make_unique<const LabelStatement>( n->GetLabel() )
        );
    }

    void CallCanon::visit( const MoveStatement* n )
    {
        n->dst->Accept( this );
        std::unique_ptr<const IRExpression> destination = std::move( prevExp );

        n->src->Accept( this );
        std::unique_ptr<const IRExpression> source = std::move( prevExp );

        updateLastStm(
                std::make_unique<const MoveStatement>(
                        destination.release(),
                        source.release()
                )
        );
    }

    void CallCanon::visit( const SeqStatement* n )
    {
        n->left->Accept( this );
        std::unique_ptr<const IRStatement> nLeft = std::move( prevStm );

        n->right->Accept( this );
        std::unique_ptr<const IRStatement> nRight = std::move( prevStm );

        updateLastStm(
                std::make_unique<const SeqStatement>(
                        nLeft.release(),
                        nRight.release()
                )
        );
    }

    void CallCanon::visit( const IRExpList* Explist )
    {
        auto newList = std::make_unique<IRExpList>();
        const auto& arguments = Explist->list;
        for( const auto& arg : arguments ) {
            arg->Accept( this );
            newList->Add( std::move( prevExp ) );
        }

        updateLastExpList( std::move( newList ) );
    }

//    void CallCanon::visit( const CStmList* list )
//    {
//        auto newList = std::make_unique<CStmList>();
//        const auto& arguments = list->GetStatements();
//        for( const auto& arg : arguments ) {
//            arg->Accept( this );
//            newList->Add( std::move( prevStm ) );
//        }
//
//        updateLastStmList( std::move( newList ) );
//    }

}
