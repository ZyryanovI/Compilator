#include "ESeqCanon.h"
#include <cassert>
#include <iostream>


namespace IRT
{

    std::unique_ptr<const IRStatement> ESeqCanon::CanonicalTree()
    {
        return std::move( CanonicalStmTree() );
    }

    std::unique_ptr<const IRStatement> ESeqCanon::CanonicalStmTree()
    {
        return std::move( prevStm );
    }

    std::unique_ptr<const IRExpression> ESeqCanon::CanonicalExpTree()
    {
        return std::move( prevExp );
    }

    void ESeqCanon::updateLastExp( const IRExpression* newLastExp )
    {
        {prevExp = std::move( std::unique_ptr<const IRExpression>( newLastExp ) );}
    }

    void ESeqCanon::updateLastExp( std::unique_ptr<const IRExpression> newLastExp )
    {
        prevExp = std::move( newLastExp );
    }

    void ESeqCanon::updateLastExpList( std::unique_ptr<const IRExpList> newLastExpList )
    {
        prevExpList = std::move( newLastExpList );
    }

    void ESeqCanon::updateLastStm( std::unique_ptr<const IRStatement> newLastStm )
    {
        prevStm = std::move( newLastStm );
    }

    std::unique_ptr<const IRExpression> ESeqCanon::canonizeExpSubtree( std::unique_ptr<const IRExpression> exp ) const
    {
        ESeqCanon visitor;
        exp->Accept( &visitor );
        return visitor.CanonicalExpTree();
    }

    std::unique_ptr<const IRStatement> ESeqCanon::canonizeStmSubtree( std::unique_ptr<const IRStatement> stm ) const
    {
        ESeqCanon visitor;
        stm->Accept( &visitor );
        return visitor.CanonicalStmTree();
    }

    bool ESeqCanon::areCommuting( const IRStatement* stm, const IRExpression* exp )
    {
        assert( stm != nullptr && exp != nullptr );
        auto expStm = dynamic_cast<const ExpStatement* >( stm );
        bool isStmEmpty = expStm != nullptr &&
                          dynamic_cast<const ConstExpression* >( expStm->GetExp() ) != nullptr;

        return isStmEmpty ||
               dynamic_cast<const ConstExpression*>( exp ) != nullptr ||
               dynamic_cast<const NameExpression*>( exp ) != nullptr;
    }

    const ESeqExpression* ESeqCanon::castToESeqExp( const IRExpression* exp )
    {
        return dynamic_cast<const ESeqExpression*>( exp );
    }

    void ESeqCanon::visit( const ConstExpression* n )
    {
        updateLastExp( std::make_unique<const ConstExpression>( n->value ) );
    }

    void ESeqCanon::visit( const NameExpression* n )
    {
        updateLastExp( std::make_unique<const NameExpression>( n->GetLabel() ) );
    }

    void ESeqCanon::visit( const TempExpression* n )
    {
        static int numEntries = 0;
        updateLastExp( std::move( std::make_unique<const TempExpression>( n->GetTemp() ) ) );
        numEntries++;
    }

    void ESeqCanon::visit( const BinOpExpression* n )
    {
        n->left->Accept( this );
        std::unique_ptr<const IRExpression> canonLeft = std::move( prevExp );
        n->right->Accept( this );
        std::unique_ptr<const IRExpression> canonRight = std::move( prevExp );

        const ESeqExpression* eseqLeft = castToESeqExp( canonLeft.get() );
        const ESeqExpression* eseqRight = castToESeqExp( canonRight.get() );

        std::unique_ptr<const IRExpression> resultExp;
        if( eseqLeft ) {
            resultExp = std::move( std::make_unique<const ESeqExpression>(
                    eseqLeft->GetStm()->GetCopy(),
                    std::move( std::make_unique<const BinOpExpression>(
                            n->binop,
                            eseqLeft->GetExp()->GetCopy(),
                            std::move( canonRight ) ) ) ) );
            if( eseqRight ) {
                resultExp = canonizeExpSubtree( std::move( resultExp ) );
            }
        } else if( eseqRight ) {
            if( areCommuting( eseqRight->GetStm(), canonLeft.get() ) ) {
                resultExp = std::move( std::make_unique<const ESeqExpression>(
                        eseqRight->GetStm()->GetCopy(),
                        std::move( std::make_unique<const BinOpExpression>(
                                n->binop,
                                std::move( canonLeft ),
                                eseqRight->GetExp()->GetCopy() ) ) ) );
            } else {
                Temp temp;
                resultExp = std::move( std::make_unique<const ESeqExpression>(
                        std::move( std::make_unique<const MoveStatement>(
                                std::move( std::make_unique<const TempExpression>( temp ) ),
                                std::move( canonLeft ) ) ),
                        std::move( std::make_unique<const ESeqExpression>(
                                eseqRight->GetStm()->GetCopy(),
                                std::move( std::make_unique<const BinOpExpression>(
                                        n->binop,
                                        std::move( std::make_unique<const TempExpression>( temp ) ),
                                        eseqRight->GetExp()->GetCopy() ) ) ) ) ) );
                resultExp = std::move( canonizeExpSubtree( std::move( resultExp ) ) );
            }
        } else {
            resultExp = std::move( std::make_unique<const BinOpExpression>(
                    n->binop,
                    std::move(canonLeft),
                    std::move(canonRight) ) );
        }

        updateLastExp( std::move(resultExp) );
    }

    void ESeqCanon::visit( const MemExpression* n )
    {
        n->expr->Accept( this );
        std::unique_ptr<const IRExpression> canonAddr = std::move( prevExp );

        const ESeqExpression* eseqAddr = castToESeqExp( canonAddr.get() );
        std::unique_ptr<const IRExpression> resultExp = nullptr;
        if( eseqAddr ) {
            resultExp = std::move( std::make_unique<const ESeqExpression>(
                    eseqAddr->GetStm()->GetCopy(),
                    std::move( std::make_unique<MemExpression>( eseqAddr->GetExp()->GetCopy() ) ) ) );
        } else {
            resultExp = std::move( std::make_unique<const MemExpression>( std::move(canonAddr) ) );
        }
        updateLastExp( std::move(resultExp) );
    }

    void ESeqCanon::visit( const CallExpression* n )
    {
        n->func->Accept( this );
        std::unique_ptr<const IRExpression> canonFunc = std::move( prevExp );

        n->args->Accept( this );
        std::vector<std::unique_ptr<const IRStatement>> newStms;

        std::unique_ptr<IRExpList> newArgs = std::make_unique<IRExpList>();
        std::unique_ptr<const IRExpList> canonArgList = std::move( prevExpList );
        for( auto& canonArg : canonArgList->list ) {
            const ESeqExpression* eseqArg = castToESeqExp( canonArg.get() );
            if( eseqArg ) {
                newStms.push_back( eseqArg->GetStm()->GetCopy() );
            }

            Temp temp;
            newArgs->Add( new TempExpression( temp ) );

            std::unique_ptr<const IRExpression> moveSrcExp;
            if( eseqArg ) {
                moveSrcExp = eseqArg->GetExp()->GetCopy();
            } else {
                moveSrcExp = canonArg->GetCopy();
            }
            std::unique_ptr<const IRStatement> moveStm = std::move( std::make_unique<const MoveStatement>(
                    std::move( std::make_unique<const TempExpression>( temp ) ),
                    std::move( moveSrcExp ) ) );
            newStms.push_back( std::move( moveStm ) );
        }

        std::unique_ptr<const IRExpression> resultExp;
        if( !newStms.empty() ) {
            std::unique_ptr<const IRStatement> suffStm = std::move( newStms.back() );
            newStms.pop_back();
            for( int i = newStms.size() - 1; i >= 0 ; i-- ) {
                suffStm = std::move( std::make_unique<const SeqStatement>(
                        std::move( newStms[i] ) ,
                        std::move( suffStm )));
            }

            resultExp = std::move( std::make_unique<const ESeqExpression>(
                    std::move( suffStm ),
                    std::move( std::make_unique<const CallExpression>(
                            std::move( canonFunc ),
                            std::move( newArgs ) ) ) ) );
        } else {
            resultExp = std::move( std::make_unique<const CallExpression>(
                    std::move( canonFunc ),
                    std::move( canonArgList ) ) );
        }

        updateLastExp( std::move( resultExp ) );
    }

    void ESeqCanon::visit( const ESeqExpression* n )
    {
        n->stm->Accept( this );
        std::unique_ptr<const IRStatement> canonStm = std::move( prevStm );
        n->expr->Accept( this );
        std::unique_ptr<const IRExpression> canonExp = std::move( prevExp );

        const ESeqExpression* eseqExp = castToESeqExp( canonExp.get() );
        std::unique_ptr<const IRExpression> resultExp;
        if( eseqExp ) {
            resultExp = std::move( std::make_unique<const ESeqExpression>(
                    std::move( std::make_unique<const SeqStatement>(
                            std::move( canonStm ),
                            eseqExp->GetStm()->GetCopy() ) ),
                    eseqExp->GetExp()->GetCopy() ) );
        } else {
            resultExp = std::move( std::make_unique<const ESeqExpression>(
                    std::move( canonStm ),
                    std::move( canonExp ) ) );
        }
        updateLastExp( std::move( resultExp ) );
    }

    void ESeqCanon::visit( const ExpStatement* n )
    {
        n->exp->Accept( this );
        std::unique_ptr<const IRExpression> canonExp = std::move( prevExp );

        const ESeqExpression* eseqExp = castToESeqExp( canonExp.get() );
        std::unique_ptr<const IRStatement> resultStm;
        if( eseqExp ) {
            resultStm = std::move( std::make_unique<const SeqStatement>(
                    eseqExp->GetStm()->GetCopy(),
                    std::move( std::make_unique<const ExpStatement>( eseqExp->GetExp()->GetCopy() ) ) ) );
        } else {
            resultStm = std::move( std::make_unique<const ExpStatement>( std::move( canonExp ) ) );
        }

        updateLastStm( std::move( resultStm ) );
    }

    void ESeqCanon::visit( const CJumpStatement* n )
    {
        n->left->Accept( this );
        std::unique_ptr<const IRExpression> canonLeft = std::move( prevExp );
        n->right->Accept( this );
        std::unique_ptr<const IRExpression> canonRight = std::move( prevExp );

        const ESeqExpression* eseqLeft = castToESeqExp( canonLeft.get() );
        const ESeqExpression* eseqRight = castToESeqExp( canonRight.get() );

        std::unique_ptr<const IRStatement> resultStm;

        if( eseqLeft ) {
            resultStm = std::move( std::make_unique<const CJumpStatement>(
                    n->rel,
                    eseqLeft->GetExp()->GetCopy(),
                    std::move( canonRight ),
                    n->if_left,
                    n->if_right ) );
            if( eseqRight ) {
                resultStm = std::move( canonizeStmSubtree( std::move( resultStm ) ) );
            }

            resultStm = std::move( std::make_unique<const SeqStatement>(
                    eseqLeft->GetStm()->GetCopy(),
                    std::move( resultStm ) ) );
        } else if( eseqRight ) {
            if( areCommuting( eseqRight->GetStm(), canonLeft.get() ) ) {
                resultStm = std::move( std::make_unique<const SeqStatement>(
                        eseqRight->GetStm()->GetCopy(),
                        std::move( std::make_unique<const CJumpStatement>(
                                n->rel,
                                std::move( canonLeft ),
                                eseqRight->GetExp()->GetCopy(),
                                n->if_left,
                                n->if_right ) ) ) );
            } else {
                Temp temp;
                resultStm = std::move( std::make_unique<const SeqStatement>(
                        std::move( std::make_unique<const MoveStatement>(
                                std::move( std::make_unique<const TempExpression>( temp ) ),
                                std::move( canonLeft ) ) ),
                        std::move( std::make_unique<const SeqStatement>(
                                eseqRight->GetStm()->GetCopy(),
                                std::move( std::make_unique<const CJumpStatement>(
                                        n->rel,
                                        std::move( std::make_unique<const TempExpression>( temp ) ),
                                        eseqRight->GetExp()->GetCopy(),
                                        n->if_left,
                                        n->if_right ) ) ) ) ) );
            }
        } else {
            resultStm = std::move( std::make_unique<const CJumpStatement>(
                    n->rel,
                    std::move( canonLeft ),
                    std::move( canonRight ),
                    n->if_left,
                    n->if_right ) );
        }

        updateLastStm( std::move( resultStm ) );
    }

    void ESeqCanon::visit( const JumpStatement* n )
    {
        updateLastStm( n->GetCopy() );
    }

    void ESeqCanon::visit( const LabelStatement* n )
    {
        updateLastStm( n->GetCopy() );
    }

    void ESeqCanon::visit( const MoveStatement* n )
    {
        n->dst->Accept( this );
        std::unique_ptr<const IRExpression> canonDest = std::move( prevExp );
        n->src->Accept( this );
        std::unique_ptr<const IRExpression> canonSrc = std::move( prevExp );

        const ESeqExpression* eseqDest = castToESeqExp( canonDest.get() );
        const ESeqExpression* eseqSrc = castToESeqExp( canonSrc.get() );

        std::unique_ptr<const IRStatement> resultStm;
        if( eseqDest ) {
            resultStm = std::move( std::make_unique<const MoveStatement>(
                    eseqDest->GetExp()->GetCopy(),
                    std::move( canonSrc ) ) );
            if( eseqSrc ) {
                resultStm = std::move( canonizeStmSubtree( std::move( resultStm ) ) );
            }
            resultStm = std::move( std::make_unique<const SeqStatement>(
                    eseqDest->GetStm()->GetCopy(),
                    std::move( resultStm ) ) );
        } else if( eseqSrc ) {
            if( areCommuting( eseqSrc->GetStm(), canonDest.get() ) ) {
                resultStm = std::move( std::make_unique<const SeqStatement>(
                        eseqSrc->GetStm()->GetCopy(),
                        std::move( std::make_unique<const MoveStatement>(
                                std::move( canonDest ),
                                eseqSrc->GetExp()->GetCopy() ) ) ) );
            } else {
                Temp temp;
                resultStm = std::move( std::make_unique<const SeqStatement>(
                        std::move( std::make_unique<const SeqStatement>(
                                eseqSrc->GetStm()->GetCopy(),
                                std::move( std::make_unique<const MoveStatement>(
                                        std::move( std::make_unique<const TempExpression>( temp ) ),
                                        eseqSrc->GetExp()->GetCopy() ) ) ) ),
                        std::move( std::make_unique<const MoveStatement>(
                                std::move( canonDest ),
                                std::move( std::make_unique<const TempExpression>( temp ) ) ) ) ) );
            }
        } else {
            resultStm = std::move( std::make_unique<const MoveStatement>(
                    MoveStatement(
                            std::move( canonDest ),
                            std::move( canonSrc ) ) ) );
        }

        updateLastStm( std::move( resultStm ) );
    }

    void ESeqCanon::visit( const SeqStatement* n )
    {
        n->left->Accept( this );
        std::unique_ptr<const IRStatement> canonLeft = std::move( prevStm );
        n->right->Accept( this );
        std::unique_ptr<const IRStatement> canonRight = std::move( prevStm );

        updateLastStm( std::move( std::make_unique<const SeqStatement>(
                std::move( canonLeft ),
                std::move( canonRight ) ) ) );
    }

    void ESeqCanon::visit( const IRExpList* expList )
    {
        std::unique_ptr<IRExpList> newExpList( new IRExpList );
        for( auto& expression : expList->list ) {
            expression->Accept( this );
            newExpList->Add( std::move(prevExp) );
        }

        updateLastExpList( std::move( newExpList ) );
    }

//    void ESeqCanon::visit( const CStmList* stmList )
//    {
//        assert( false );
//    }

}
