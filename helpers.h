#ifndef helpers_h
#define helpers_h

#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "move.h"
using Bitboard = uint64_t;
using Square = int;

const Bitboard oneBB = Bitboard(1);

namespace myEngine {
//namespace helpers {
//    
//    void init();
//    std::string pretty (Bitboard b);
//    }
    constexpr Bitboard FileABB = 0x0101010101010101ULL;
    constexpr Bitboard FileBBB = FileABB << 1;
    constexpr Bitboard FileCBB = FileABB << 2;
    constexpr Bitboard FileDBB = FileABB << 3;
    constexpr Bitboard FileEBB = FileABB << 4;
    constexpr Bitboard FileFBB = FileABB << 5;
    constexpr Bitboard FileGBB = FileABB << 6;
    constexpr Bitboard FileHBB = FileABB << 7;
    
    constexpr Bitboard Rank1BB = 0xFF;
    constexpr Bitboard Rank2BB = Rank1BB << (8 * 1);
    constexpr Bitboard Rank3BB = Rank1BB << (8 * 2);
    constexpr Bitboard Rank4BB = Rank1BB << (8 * 3);
    constexpr Bitboard Rank5BB = Rank1BB << (8 * 4);
    constexpr Bitboard Rank6BB = Rank1BB << (8 * 5);
    constexpr Bitboard Rank7BB = Rank1BB << (8 * 6);
    constexpr Bitboard Rank8BB = Rank1BB << (8 * 7);
    


    //enum Square : int {
    //    A1, B1, C1, D1, E1, F1, G1, H1,
    //    A2, B2, C2, D2, E2, F2, G2, H2,
    //    A3, B3, C3, D3, E3, F3, G3, H3,
    //    A4, B4, C4, D4, E4, F4, G4, H4,
    //    A5, B5, C5, D5, E5, F5, G5, H5,
    //    A6, B6, C6, D6, E6, F6, G6, H6,
    //    A7, B7, C7, D7, E7, F7, G7, H7,
    //    A8, B8, C8, D8, E8, F8, G8, H8,
    //    NONE,
    //
    //    SQUARE_ZERO = 0,
    //    SQUARE_NB   = 64
    //};
    enum pieceCodes : char {
        P, N, B, R, Q, K, p, n, b, r, q, k, slash
    };
    constexpr pieceCodes hashing (char in) {
        if (in == 'r') return r;
        if (in == 'n') return n;
        if (in == 'b') return b;
        if (in == 'q') return q;
        if (in == 'k') return k;
        if (in == 'p') return p;
        if (in == 'P') return P;
        if (in == 'R') return R;
        if (in == 'N') return N;
        if (in == 'B') return B;
        if (in == 'Q') return Q;
        if (in == 'K') return K;
        if (in == '/') return slash;
        return slash;
    }
    
    enum Direction : int {
        NORTH = 8,
        EAST  = 1,
        SOUTH = -NORTH,
        WEST  = -EAST,
    
        NORTH_EAST = NORTH + EAST,
        SOUTH_EAST = SOUTH + EAST,
        SOUTH_WEST = SOUTH + WEST,
        NORTH_WEST = NORTH + WEST
    };
    
    enum File : int {
        FILE_A,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H,
        FILE_NB
    };
    
    enum Rank : int {
        RANK_1,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8,
        RANK_NB
    };

    static const char *STC[] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    };

    Bitboard intToBitBoard(int square);

    Bitboard strToBitBoard(char file, char rank);

    void printBitBoard(Bitboard);

    void printMove(Move);
    int squareToInt(std::string square);

    constexpr int BlackPawnPositionBB[64] = {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    inline int WhitePawnPositionBB[64];
    inline const auto WP = []() constexpr {
    for(int i = 0; i < 64; ++i) WhitePawnPositionBB[i] = BlackPawnPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    constexpr int BlackKnightPositionBB[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    };

    inline int WhiteKnightPositionBB[64];
    inline const auto WK = []() constexpr {
    for(int i = 0; i < 64; ++i) WhiteKnightPositionBB[i] = BlackKnightPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    constexpr int BlackBishopPositionBB[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    };

    inline int WhiteBishopPositionBB[64];
    inline const auto WB = []() constexpr {
    for(int i = 0; i < 64; ++i) WhiteBishopPositionBB[i] = BlackBishopPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    constexpr int BlackRookPositionBB[64] = {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    };
    inline int WhiteRookPositionBB[64];
    inline const auto WR = []() constexpr {
    for(int i = 0; i < 64; ++i) WhiteRookPositionBB[i] = BlackRookPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    constexpr int BlackQueenPositionBB[64] = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };

    inline int WhiteQueenPositionBB[64];
    inline const auto WQ = []() constexpr {
    for(int i = 0; i < 64; ++i) WhiteQueenPositionBB[i] = BlackQueenPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    constexpr int BlackKingPositionBB[64] = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    };

    inline int WhiteKingPositionBB[64];
    inline const auto WKG = []() constexpr {
    for(int i = 0; i < 64; ++i) WhiteKingPositionBB[i] = BlackKingPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    constexpr int BlackKingEndPositionBB[64] = {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    };

    inline int WhiteKingEndPositionBB[64];
    inline const auto WKE = []() constexpr {
    for(int i = 0; i < 64; ++i) WhiteKingEndPositionBB[i] = BlackKingEndPositionBB[56 - (i/8)*8 + i%8]; 
    return 0; 
    }();

    const int *getPiecePositionTable(int piece, bool colour, bool endgame);

};

#endif