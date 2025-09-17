#include "preMoveGen.h"


Bitboard * genKnightAttackMask() {
    static Bitboard knightAttackMask[64];
    Bitboard attackMask = 0;
    for (int square = 0; square < 64; square++) {
        Bitboard squareBB = oneBB << square;
        if (square %8 != 7) {
            attackMask |= squareBB << (NORTH + NORTH_EAST); // 17
            attackMask |= squareBB >> -(SOUTH + SOUTH_EAST); // -15
        }
        if (square %8 < 6) {
            attackMask |= squareBB << (EAST + NORTH_EAST); // 10
            attackMask |= squareBB >> -(EAST + SOUTH_EAST); // -6
        }
        if (square %8 != 0) {
            attackMask |= squareBB << (NORTH + NORTH_WEST); //15
            attackMask |= squareBB >> -(SOUTH + SOUTH_WEST); //-17
        }
        if (square %8 > 1) {
            attackMask |= squareBB << (WEST + NORTH_WEST); // 6
            attackMask |= squareBB >> -(WEST + SOUTH_WEST); //-10
        }
        knightAttackMask[square] = attackMask;
        attackMask = 0;
    }
    return knightAttackMask;
}


void genPossibleBishopMoves(){
    for (int square = 0; square< 64; square ++){
        uint64_t attackMask = bishopMasks[square];
        int relevantBits = getBitCount(attackMask);
        int occupancyIndex = (1 << relevantBits);
        for (int index = 0; index < occupancyIndex; index ++){
            uint64_t occupancy = set_occupancy(index, relevantBits, attackMask);
            int magicIndex = (occupancy * bishopMagics[square]) >> (64-relevantBits);
            bishopAttackMasks[square][magicIndex] = genBishopAttacks(square, occupancy);
        }
    }
}

void genPossibleRookMoves(){
    for (int square = 0; square< 64; square ++){
        uint64_t attackMask = rookMasks[square];
        int relevantBits = getBitCount(attackMask);
        int occupancyIndex = (1 << relevantBits);
        for (int index = 0; index < occupancyIndex; index ++){
            uint64_t occupancy = set_occupancy(index, relevantBits, attackMask);
            int magicIndex = occupancy * rookMagics[square] >> (64-relevantBits);
            uint64_t attacks = genRookAttacks(square, occupancy);
            RookAttackMasks[square][magicIndex] = genRookAttacks(square, occupancy);
        }
    }
}

Bitboard * genPossibleKingMoves() {
    static Bitboard kingAttackMask[64];
    Bitboard attackMask = 0;
    for (int square = 0; square< 64; square ++){ 
        Bitboard squareBB = oneBB << square;
        attackMask |= squareBB << NORTH; //straight up
        attackMask |= squareBB >> NORTH; //straight down
        if (square %8 != 7) { //not touching right edge
            attackMask |= squareBB << NORTH_EAST;
            attackMask |= squareBB << EAST;
            attackMask |= squareBB >> -SOUTH_EAST;
        }
        if (square %8 != 0) { //not touching left edge
            attackMask |= squareBB << NORTH_WEST;
            attackMask |= squareBB >> -WEST;
            attackMask |= squareBB >> -SOUTH_WEST;
        }
        kingAttackMask[square] = attackMask;
        attackMask = 0;
    }
    return kingAttackMask;
}