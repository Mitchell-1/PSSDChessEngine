#pragma once
#include <iostream>
#include "helpers.h"

const int index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

const uint64_t debruijn64 = uint64_t(0x03f79d71b4cb0a89);

namespace bitboardHelpers {
    constexpr void popBit(Bitboard& board, Square square) {
        Bitboard squareBB = oneBB << square;
        board |= squareBB;
        board ^= squareBB;
    }

    constexpr void popBit(Bitboard& board, Bitboard bit) {
        board |= bit;
        board ^= bit;
    }

    constexpr void flipBit(Bitboard& board, Square square) {
        Bitboard squareBB = oneBB << square;
        board ^= squareBB;
    }

    constexpr void flipBit(Bitboard& board, Bitboard bit) {
        board ^= bit;
    }

    constexpr void setBit(Bitboard& board, Square square) {
        Bitboard squareBB = oneBB << square;
        board ^= squareBB;
        board |= squareBB;
    }

    constexpr void setBit(Bitboard& board, Bitboard bit) {
        board ^= bit;
        board |= bit;
    }

    constexpr Square getLSB(Bitboard& board) {
        if (board != 0){
            Square lsb =  (index64[((board ^ (board-1)) * debruijn64) >> 58]);
            bitboardHelpers::popBit(board, lsb);
            return lsb;
        } else {
            return 404;
        }
    }

    constexpr void updateBoard(Bitboard& board, int init, int target){
        popBit(board, init);
        setBit(board, target);
    }
    constexpr void updateBoard(Bitboard& board, int init){
        flipBit(board, init);
    }
}