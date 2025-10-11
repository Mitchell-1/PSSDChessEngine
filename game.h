#ifndef game_h
#define game_h

#include "helpers.h"
#include "boardHelpers.h"
#include "preMoveGen.h"
#include "move.h"
#include <iostream>
#include <vector>
#include <bitset>

// Game class to handle all interactions with the game state
// and board representations.
class Game {    
    public:
        Bitboard whiteBoards[7] = {0, 0, 0, 0, 0, 0, 0};
        Bitboard blackBoards[7] = {0, 0, 0, 0, 0, 0, 0};
        Bitboard whiteAttacks[7] = {0, 0, 0, 0, 0, 0, 0};
        Bitboard blackAttacks[7] = {0, 0, 0, 0, 0, 0, 0};
        Bitboard whiteAttack = 0;
        Bitboard blackAttack = 0;
        Bitboard mainBoard = 0;
        Bitboard whiteBoard = 0;
        Bitboard blackBoard = 0;

        std::vector<Bitboard> WhitePHistory;
        std::vector<Bitboard> BlackPHistory;

        Game(std::string fen){
            fenToBitBoards(fen);
            WhitePHistory.reserve(30);
            BlackPHistory.reserve(30);
        }
        void fenToBitBoards(std::string fen);
        void updateMainBoards();
        int getPieceBySquare(Bitboard, int) const;
        void genAttacks();
        void updateAttacks(Move move);
        bool makeMove(Move& move);
        void unMakeMove(Move& move);
        bool isPosOk(Move& move);
        void recieveMove(std::string moveStr);
        Bitboard whiteKingCastle = 112;
        Bitboard WhiteKingRook = 160;
        bool canWhiteKingCastle = false;
        Bitboard whiteQueenCastle = 28;
        Bitboard WhiteQueenRook = 9;
        bool canWhiteQueenCastle = false;
        Bitboard blackKingCastle = 8070450532247928832;
        Bitboard blackKingRook = 0xA000000000000000;
        bool canBlackKingCastle = false;
        Bitboard blackQueenCastle = 2017612633061982208;
        bool canBlackQueenCastle = false;
        Bitboard blackQueenRook = 648518346341351424;
        bool turn = 0;
        bool debug = false;
};

#endif