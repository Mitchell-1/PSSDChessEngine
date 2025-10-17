#include "evaluate.h"
#include <climits>

int PieceValues[5]  = {
    100, // pawn
    300, // knight
    320, // bishop
    500, // rook
    900  // queen
};

// Evaluates the board position and returns a score.
// Positive score indicates advantage for white, negative for black.
// Verbosity controls the amount of debug information printed.
// If moves is provided and is zero, it indicates no legal moves are available.
// Evaluation function utilizes material count, positional advantages, and attack coverage to generate score.
int Evaluate::evaluateBoard(const Game &game, int verbosity, int moves) {

    // If there are no legal moves, return mate or stalemate
    if (moves == 0) {
        if (verbosity >= 1) {
            std::cout << "Optional Moves passed, returning mate or stalemate" << std::endl;
        }
        if (game.turn == 0) {
            if (game.whiteBoards[5] & game.blackAttack) {
                return INT_MIN; // White to move and in check with no legal moves, black mates
            }
            return 0; // White to move and no moves, black stalemates
        } else {
            if (game.blackBoards[5] & game.whiteAttack) {
                return INT_MAX; // Black to move and in check with no legal moves, white mates
            }
            return 0; // Black to move and no moves, white stalemates
        }
    }

    // Fifty full moves without a capture is a draw.
    if (game.movesWithoutCapture >= 100) {
        return 0; 
    }

    // Count the material score for both sides, adding for white and subtracting for black.
    int materialScore = 0;
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
        const int *positionTableW = myEngine::getPiecePositionTable(piece, 0, endgame);
        const int *positionTableB = myEngine::getPiecePositionTable(piece, 1, endgame);
        while (whitePieceBB) {
            Square square = bitboardHelpers::getLSB(whitePieceBB);
            positionalScore += positionTableW[square];
        }
        while (blackPieceBB) {
            Square square = bitboardHelpers::getLSB(blackPieceBB);
            positionalScore -= positionTableB[square];
        }
    }
    // Attack score is calculated by counting the number of squares attacked by each side
    // and adding a small bonus for each square attacked.
    int attackScore = 0;
    attackScore += bitboardHelpers::getBitCount(game.whiteAttack) * 5;
    attackScore -= bitboardHelpers::getBitCount(game.blackAttack) * 5;

    if (verbosity >= 1) {
        std::cout << "Material Score: " << materialScore << std::endl;
        std::cout << "Positional Score: " << positionalScore << std::endl;
        std::cout << "Attack Score: " << attackScore << std::endl;
    }

        return materialScore + positionalScore + attackScore; // small bonus for side to move
};