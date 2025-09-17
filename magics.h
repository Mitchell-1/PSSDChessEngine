#ifndef magics_h
#define magics_h

#include<iostream>
#include "helpers.h"
#include "boardHelpers.h"
#include <cstring>
extern unsigned int state;
unsigned int getRandomNumber();
uint64_t getRandomU64();
static uint64_t getMagicNumCandidate(){
    return getRandomU64() & getRandomU64() & getRandomU64();
}

uint64_t getRandomU64();
int getBitCount(uint64_t board);

extern uint64_t rookMagics[64];
extern uint64_t RookAttackMasks[64][4096];
extern uint64_t bishopMagics[64];
extern uint64_t bishopAttackMasks[64][512];
extern uint64_t* bishopMasks;
extern uint64_t * rookMasks;
const int bishopBitCounts[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};
const int rookBitCounts[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};

uint64_t * genBishopMask();
uint64_t * genRookMask();
uint64_t * genQueenMask(uint64_t * rook, uint64_t * bishop);
uint64_t set_occupancy(int index, int numBits, uint64_t attackMask);
uint64_t findMagicNum(int square, int relevantBits, int piece);
void initMagicNumbers();

uint64_t * genBishopMask();
uint64_t * genRookMask();

uint64_t genBishopAttacks(int square, uint64_t blockers);
uint64_t genRookAttacks(int square, uint64_t blockers);

uint64_t get_bishop(int square, uint64_t occupancy);
uint64_t get_rook(int square, uint64_t occupancy);

#endif