#ifndef preMoveGen_h
#define preMoveGen_h
#include "helpers.h"
#include "magics.h"


using namespace myEngine;

Bitboard * genKnightAttackMask();
void genPossibleBishopMoves();
void genPossibleRookMoves();
Bitboard *  genPossibleKingMoves();

static Bitboard *knightAttackMask = genKnightAttackMask();
static Bitboard *kingAttackMask = genPossibleKingMoves();


#endif
