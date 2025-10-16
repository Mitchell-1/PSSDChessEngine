#include "evaluate.h"
#include <climits>

int PieceValues[5]  = {
    100, // pawn
    300, // knight
    320, // bishop
    500, // rook
    900  // queen
};

int Evaluate::evaluateBoard(const Game &game, int moves){
    if (moves == 0) {
        if (game.turn == 0) {
            if (game.whiteBoards[5] & game.blackAttack) {
                return INT_MIN; // white to move and in check with no legal moves, black mates
            }
            return 0; // white to move and no moves, black stalemates
        } else {
            if (game.blackBoards[5] & game.whiteAttack) {
                return INT_MAX; // black to move and in check with no legal moves, white mates
            }
            return 0; // black to move and no moves, white stalemates
        }
    }
        int materialScore = 0;

        // Count the material score for both sides, adding for white and subtracting for black.
        for (int piece = 0; piece < 6; piece++) {
            //select both sides piece bitboards for the current piece.
            Bitboard whitePieceBB = game.whiteBoards[piece];
            Bitboard blackPieceBB = game.blackBoards[piece];
            
            // Count the pieces by clearing the least significant bit until the bitboard is empty.
            // Score is the piece value multiplied by the number of pieces.
            while (whitePieceBB) {
                materialScore += PieceValues[piece];
                whitePieceBB &= whitePieceBB - 1; // clear the least significant bit
            }
            while (blackPieceBB) {
                materialScore -= PieceValues[piece];
                blackPieceBB &= blackPieceBB - 1; // clear the least significant bit
            }
        }

        // Positional score is calculated by using the positional tables for each piece
        // as defined in Helpers.h. The tables are indexed by the square of each piece.
        int positionalScore = 0;
        bool endgame = bitboardHelpers::getBitCount(game.whiteBoard) < 5 || bitboardHelpers::getBitCount(game.blackBoard) < 5 || bitboardHelpers::getBitCount(game.mainBoard) < 20;
        for (int piece = 0; piece < 6; piece++) {
            Bitboard whitePieceBB = game.whiteBoards[piece];
            Bitboard blackPieceBB = game.blackBoards[piece];
            while (whitePieceBB) {
                Square square = bitboardHelpers::getLSB(whitePieceBB);
                const int *positionTable = myEngine::getPiecePositionTable(piece, 0, endgame);
                positionalScore += positionTable[square];
            }
            while (blackPieceBB) {
                Square square = bitboardHelpers::getLSB(blackPieceBB);
                const int *positionTable = myEngine::getPiecePositionTable(piece, 1, endgame);
                positionalScore -= positionTable[square];
            }
        }

        return materialScore + positionalScore;
};