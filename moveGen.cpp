#include "moveGen.h"

// Template wrapper allows for efficient filtering of move generation allowing for 
// specific move types to be generated without overhead of checking types.
template<genType Type>
Move * generateWhitePawnMoves(const Game& position, Move* moveList) {
    Bitboard Pawns = position.whiteBoards[0];
    Bitboard PawnsNotA = Pawns & ~myEngine::FileABB;
    Bitboard PawnsNotH = Pawns & ~myEngine::FileHBB;
    Bitboard Rank2Pawns = Pawns & myEngine::Rank2BB;
    Square from;
    Square to;

    // Generates all non-capturing pawn moves.
    if (Type != CAPTURES) {
        Bitboard blockers = position.mainBoard >> 8;
        bitboardHelpers::popBit(Pawns, blockers);
        Rank2Pawns = Pawns & myEngine::Rank2BB;
        // Generates single push Pawn moves
        while (Pawns != 0){
            from = bitboardHelpers::getLSB(Pawns);
            to = from + 8;
            if (oneBB << from & myEngine::Rank7BB) {
                for (int promo = 0; promo < 4; promo ++) {
                    *moveList ++ = Move(from, to, promo, 1, 0, 7, 0, 0);
                }
                
            } else {
                *moveList ++ = Move(from, to, 0, 0, 0, 7, 0, 0);
            }
        }
        // Generates double push Pawn moves.
        blockers |= position.mainBoard >> 16;
        bitboardHelpers::popBit(Rank2Pawns, blockers);
        while (Rank2Pawns) {
            
            from = bitboardHelpers::getLSB(Rank2Pawns);
            to = from + 16;
            *moveList ++ = Move(from, to, 0, 0, 0, 7, 0, 1);
        }
        
    }
    // Generates all capturing pawn moves.
    if (Type == CAPTURES || Type == LEGAL) {
        while (PawnsNotA != 0)
        {
            from = bitboardHelpers::getLSB(PawnsNotA);
            to = myEngine::NORTH_WEST + from;
            if (((oneBB << to) & position.blackBoard) | ((oneBB << to) & position.blackBoards[6])){
                int capture = position.getPieceBySquare((oneBB << to), 1);
                if (oneBB << from & myEngine::Rank7BB) {
                    for (int promo = 0; promo < 4; promo ++) {
                        *moveList ++ = Move(from, to, promo, 1, 0, capture, 0, 0);
                    }
                } else {
                    *moveList ++ = Move(from, to, 0, 0, 0, capture, 0, 0);
                }
            }
        }
        while (PawnsNotH != 0)
        {
            from = bitboardHelpers::getLSB(PawnsNotH);
            to = myEngine::NORTH_EAST + from;
            if (((oneBB << to) & position.blackBoard) || ((oneBB << to) & position.blackBoards[6])){
                int capture = position.getPieceBySquare((oneBB << to), 1);
                if (oneBB << from & myEngine::Rank7BB) {
                    for (int promo = 0; promo < 4; promo ++) {
                        *moveList ++ = Move(from, to, promo, 1, 0, capture, 0, 0);
                    }
                } else {
                    *moveList ++ = Move(from, to, 0, 0, 0, capture, 0, 0);
                }
            }
        }
        
    }
    return moveList;
};

template<genType Type>
Move * generateBlackPawnMoves(const Game& position, Move* moveList) {
    Bitboard Pawns = position.blackBoards[0];
    Bitboard PawnsNotA = Pawns & ~myEngine::FileABB;
    Bitboard PawnsNotH = Pawns & ~myEngine::FileHBB;
    Bitboard Rank7Pawns = Pawns & myEngine::Rank7BB;
    Square from;
    Square to;
    // Generates all non-capturing pawn moves.
    if (Type != CAPTURES) {
        Bitboard blockers = position.mainBoard << 8;
        bitboardHelpers::popBit(Pawns, blockers);
        Rank7Pawns = Pawns & myEngine::Rank7BB;
        // Generates single push Pawn moves
        while (Pawns != 0){
            from = bitboardHelpers::getLSB(Pawns);
            to = from - 8;
            if ((oneBB << from) & myEngine::Rank2BB) {
                for (int promo = 0; promo < 4; promo ++) {
                    *moveList ++ = Move(from, to, promo, 1, 0, 7, 1, 0);
                }
            } else {
                *moveList ++ = Move(from, to, 0, 0, 0, 7, 1, 0);
            }
        }
        // Generates double push Pawn moves.
        blockers |= position.mainBoard << 16;
        bitboardHelpers::popBit(Rank7Pawns, blockers);
        while (Rank7Pawns) {
            
            from = bitboardHelpers::getLSB(Rank7Pawns);
            to = from - 16;
            *moveList ++ = Move(from, to, 0, 0, 0, 7, 1, 1);
        }
        
    }

    // Generates all capturing pawn moves.
    if (Type == CAPTURES || Type == LEGAL) {
        while (PawnsNotA != 0)
        {
            from = bitboardHelpers::getLSB(PawnsNotA);
            to = myEngine::SOUTH_WEST + from;
            if (((oneBB << to) & position.whiteBoard) | ((oneBB << to) & position.whiteBoards[6])){
                int capture = position.getPieceBySquare((oneBB << to), 0);
                if (oneBB << from & myEngine::Rank2BB) {
                    for (int promo = 0; promo < 4; promo ++) {
                        *moveList ++ = Move(from, to, promo, 1, 0, capture, 1, 0);
                    }
                } else {
                    *moveList ++ = Move(from, to, 0, 0, 0, capture, 1, 0);
                }
            }
        }
        while (PawnsNotH != 0)
        {
            from = bitboardHelpers::getLSB(PawnsNotH);
            to = myEngine::SOUTH_EAST + from;
            if (((oneBB << to) & position.whiteBoard) | ((oneBB << to) & position.whiteBoards[6])){
                int capture = position.getPieceBySquare((oneBB << to), 0);
                if (oneBB << from & myEngine::Rank2BB) {
                    for (int promo = 0; promo < 4; promo ++) {
                        *moveList ++ = Move(from, to, promo, 1, 0, capture, 1, 0);
                    }
                } else {
                    *moveList ++ = Move(from, to, 0, 0, 0, capture, 1, 0);
                }
            }
        }
        
    }
    return moveList;
};


template<genType Type>
Move * generateKnightMoves(const Game& position, Move* moveList) {
    Bitboard friendlyBoard, enemyBoard, KnightBoard;
    bool enemy;
    if (position.turn) {
        KnightBoard = position.blackBoards[1];
        friendlyBoard = position.blackBoard;
        enemyBoard = position.whiteBoard;
        enemy = 0;
    } else {
        KnightBoard = position.whiteBoards[1];
        friendlyBoard = position.whiteBoard;
        enemyBoard = position.blackBoard;
        enemy = 1;
    }
    Square from;
    Square to;
    int capture = 7;
    while (KnightBoard != 0) {
        from = bitboardHelpers::getLSB(KnightBoard);
        Bitboard attacks = knightAttackMask[from];
        bitboardHelpers::popBit(attacks, friendlyBoard);
        if (Type != LEGAL && Type != CAPTURES) {
            bitboardHelpers::popBit(attacks, enemyBoard);
        } else if (Type == CAPTURES) {
            bitboardHelpers::popBit(attacks, ~enemyBoard);
        }
        
        while (attacks != 0) {
            to = bitboardHelpers::getLSB(attacks);
            if (((oneBB << to) & enemyBoard)) {
                capture = position.getPieceBySquare((oneBB<<to), enemy);
                *moveList++ = Move(from, to, 0, 0, 1, capture, position.turn, 0);
            } else {
                *moveList++ = Move(from, to, 0, 0, 1, 7, position.turn, 0);
            }
        }
    }
    return moveList;
};

template<genType Type>
Move * generateBishopMoves(const Game& position, Move* moveList) {
    Bitboard bishopBoard, enemyBoard, friendlyBoard;
    if (position.turn) {
        bishopBoard = position.blackBoards[2];
        enemyBoard = position.whiteBoard;
        friendlyBoard = position.blackBoard;
    } else {
        bishopBoard = position.whiteBoards[2];
        enemyBoard = position.blackBoard;
        friendlyBoard = position.whiteBoard;
    }
    int lsb, attackLSB;
    
    while (bishopBoard != 0) {
        lsb = bitboardHelpers::getLSB(bishopBoard);
        Bitboard attacks = get_bishop(lsb, position.mainBoard);
        bitboardHelpers::popBit(attacks, friendlyBoard);
        if (Type != LEGAL && Type != CAPTURES) {
            bitboardHelpers::popBit(attacks, enemyBoard);
        } else if (Type == CAPTURES) {
            bitboardHelpers::popBit(attacks, ~enemyBoard);
        }
        while (attacks != 0)
        {
            attackLSB = bitboardHelpers::getLSB(attacks);
            int capture = 7;
            if (enemyBoard & (oneBB << attackLSB)) {
                bool enemy = position.turn ? 0 : 1;
                capture = position.getPieceBySquare(oneBB << attackLSB, enemy);
            }
            *moveList++ = Move(lsb, attackLSB, 0, 0, 2, capture, position.turn, 0);
        }
        
    }
    return moveList;
}

template<genType Type>
Move * generateRookMoves(const Game& position, Move* moveList) {
    Bitboard rookBoard, enemyBoard, friendlyBoard;
    if (position.turn) {
        rookBoard = position.blackBoards[3];
        enemyBoard = position.whiteBoard;
        friendlyBoard = position.blackBoard;
    } else {
        rookBoard = position.whiteBoards[3];
        enemyBoard = position.blackBoard;
        friendlyBoard = position.whiteBoard;
    }

    int lsb, attackLSB;
    while (rookBoard != 0) {
        lsb = bitboardHelpers::getLSB(rookBoard);
        Bitboard attacks = get_rook(lsb, position.mainBoard);
        bitboardHelpers::popBit(attacks, friendlyBoard);
        if (Type != LEGAL && Type != CAPTURES) {
            bitboardHelpers::popBit(attacks, enemyBoard);
        } else if (Type == CAPTURES) {
            bitboardHelpers::popBit(attacks, ~enemyBoard);
        }
        while (attacks != 0)
        {
            attackLSB = bitboardHelpers::getLSB(attacks);
            int capture = 7;
            if (enemyBoard & (oneBB << attackLSB)) {
                bool enemy = position.turn ? 0 : 1;
                capture = position.getPieceBySquare(oneBB << attackLSB, enemy);
            }
            *moveList++ = Move(lsb, attackLSB, 0, 0, 3, capture, position.turn, 0);
        }
        
    }
    return moveList;
}

template<genType Type>
Move * generateQueenMoves(const Game& position, Move* moveList) {
    Bitboard queenBoard, enemyBoard, friendlyBoard;
    if (position.turn) {
        queenBoard = position.blackBoards[4];
        enemyBoard = position.whiteBoard;
        friendlyBoard = position.blackBoard;
    } else {
        queenBoard = position.whiteBoards[4];
        enemyBoard = position.blackBoard;
        friendlyBoard = position.whiteBoard;
    }

    int lsb, attackLSB;
    while (queenBoard != 0) {
        lsb = bitboardHelpers::getLSB(queenBoard);
        Bitboard attacksB = get_bishop(lsb, position.mainBoard);
        Bitboard attacksR = get_rook(lsb, position.mainBoard);
        Bitboard attacks = attacksB | attacksR;
        bitboardHelpers::popBit(attacks, friendlyBoard);
        if (Type != LEGAL && Type != CAPTURES) {
            bitboardHelpers::popBit(attacks, enemyBoard);
        } else if (Type == CAPTURES) {
            bitboardHelpers::popBit(attacks, ~enemyBoard);
        }
        while (attacks != 0)
        {
            attackLSB = bitboardHelpers::getLSB(attacks);
            int capture = 7;
            if (enemyBoard & (oneBB << attackLSB)) {
                bool enemy = position.turn ? 0 : 1;
                capture = position.getPieceBySquare(oneBB << attackLSB, enemy);
            }
            *moveList++ = Move(lsb, attackLSB, 0, 0, 4, capture, position.turn, 0);
        }
        
    }
    return moveList;
}

template<genType Type>
Move * generateKingMoves(const Game& position, Move* moveList) {
    Bitboard kingCastle, queenCastle, friendlyBoard, enemyBoard, enemyAttacks, king, king2;
    Bitboard * friendlyBoards;
    bool canKingCastle, canQueenCastle;
    if (position.turn) {
        canKingCastle = position.canBlackKingCastle;
        canQueenCastle = position.canBlackQueenCastle;
        kingCastle = position.blackKingCastle;
        queenCastle = position.blackQueenCastle;
        enemyBoard = position.whiteBoard;
        friendlyBoard = position.blackBoard;
        king = position.blackBoards[5];
        king2 = position.blackBoards[5];
        enemyAttacks = position.whiteAttack;
        //printf(" %d %d %llu %llu \n", canKingCastle, canQueenCastle, kingCastle, queenCastle);
    } else {
        canKingCastle = position.canWhiteKingCastle;
        canQueenCastle = position.canWhiteQueenCastle;
        kingCastle = position.whiteKingCastle;
        queenCastle = position.whiteQueenCastle;
        enemyBoard = position.blackBoard;
        friendlyBoard = position.whiteBoard;
        king = position.whiteBoards[5];
        king2 = position.whiteBoards[5];
        enemyAttacks = position.blackAttack;
    }
    int lsb, attackLSB;
    bool newTurn = position.turn ? 0:1;
    lsb = bitboardHelpers::getLSB(king);
    Bitboard attacks = kingAttackMask[lsb];
    bitboardHelpers::popBit(attacks, friendlyBoard);
    if (Type != LEGAL && Type != CAPTURES) {
        bitboardHelpers::popBit(attacks, enemyBoard);
    } else if (Type == CAPTURES) {
        bitboardHelpers::popBit(attacks, ~enemyBoard);
    }
    int capture;
    while(attacks != 0) {
        attackLSB = bitboardHelpers::getLSB(attacks);
        capture = 7;
        if (enemyBoard & (oneBB<<attackLSB)){
            capture = position.getPieceBySquare(oneBB << attackLSB, newTurn);
        }
        *moveList ++ = Move(lsb, attackLSB, 0, 0, 5, capture, position.turn, 0);
    } 
    if (Type != CAPTURES) {
        if (canKingCastle) {
            if (!(kingCastle & enemyAttacks) &&
                ((kingCastle & position.mainBoard) == king2)) {
                *moveList++ = Move(lsb, lsb+2, 0, 2, 5, 7, position.turn, 0);
            }
        }

        Bitboard extraSquare = position.turn ? oneBB << 57 : 2;
        if (canQueenCastle) {

            if (!(queenCastle & enemyAttacks) && 
                (((queenCastle | extraSquare) & position.mainBoard) == king2)) {
                *moveList++ = Move(lsb, lsb-2, 0, 3, 5, 7, position.turn, 0);
            }
        }
    }
    return moveList;
}


template<genType Type>
Move* generate(Game& pos, Move * moveList) {
    if (pos.turn) {
        moveList = generateBlackPawnMoves<Type>(pos, moveList);
    } else {
        moveList = generateWhitePawnMoves<Type>(pos, moveList);
    }
    moveList = generateKnightMoves<Type>(pos, moveList);
    moveList = generateBishopMoves<Type>(pos, moveList);
    moveList = generateRookMoves<Type>(pos, moveList);
    moveList = generateQueenMoves<Type>(pos, moveList);
    moveList = generateKingMoves<Type>(pos, moveList);
    return moveList;
};


template Move* generate<CAPTURES>(Game&, Move*);
template Move* generate<QUIETS>(Game&, Move*);
template Move* generate<EVASIONS>(Game&, Move*);
template Move* generate<NON_EVASIONS>(Game&, Move*);
template Move* generate<LEGAL>(Game&, Move*);

