#include "helpers.h"


Bitboard myEngine::intToBitBoard(int square){
    return ((uint64_t (1)) << square);
}

// Takes a standard square notatin (e.g. e4) and converts
// it into a bitboard with only that bit set.
Bitboard myEngine::strToBitBoard(char file, char rank) {
    char files1[9] = "abcdefgh";
    int fileNum = std::string(files1).find(file);
    int ranknum = rank - '0' - 1;
    return ranknum*8 + fileNum;
}

// Prints a bitboard to the console in an 8x8 format
// with ranks and files labelled.
void myEngine::printBitBoard(Bitboard bitboard){
    std::vector<char> files = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    //std::cout << std::bitset<64>(this->board) << std::endl;
    uint64_t square = uint64_t(1) << 56;
    //std::cout << std::bitset<64>(square) << std::endl;
    printf("\n\n");
    for (int rank = 0; rank < 8; rank++){
        printf("%d   ", 8 - rank);
        for (int file = 0; file < 8; file++){  
            bool occupied = (bitboard & square) ? 1 : 0;
            std::cout << occupied << " ";
            if(file != 7){square = square << 1;};
        }
        square = square >> 15;
        if (square == 0){square++;};
        printf("\n");
    }
    printf("\n    a b c d e f g h\n");
}

// Converts a standard square notation (e.g. e4) into
// an integer value (0-63).
int myEngine::squareToInt(std::string square){
    std::string files1 = "abcdefgh";
    char file = square[0];
    int fileNum = files1.find(file);
    int rank = square[1] - '0' - 1;
    return rank*8 + fileNum;
}

// Prints the details of a Move object to the console
void myEngine::printMove(Move move){
    const char * colour = move.colour() ? "Black  " : "White  ";
    switch (move.type())
    {
    case 0:
        printf("%s  Pawn      %s  ->  %s     %d       %d           %d\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture(), (move.special()==1)*(move.promotion()+1), move.passantable());
        break;
    case 2:
        printf("%s  Bishop    %s  ->  %s     %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture() );
        break;
    case 1:
        printf("%s  Knight    %s  ->  %s     %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture() );
        break;
    case 3:
        printf("%s  Rook      %s  ->  %s     %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture() );
        break;
    case 4:
        printf("%s  Queen     %s  ->  %s     %d       N           N\n" , colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture() );
        break;
    case 5:
        printf("%s  King      %s  ->  %s     %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture());
        break;
    default:
        break;
    }
}

// Returns the piece-square table for the given piece and colour.
const int *myEngine::getPiecePositionTable(int piece, bool colour, bool endgame){
    const int *positionTable = nullptr;
    switch (piece){
        case 0: // pawn
            return colour ? BlackPawnPositionBB : WhitePawnPositionBB;
        case 1: // knight
            return colour ? BlackKnightPositionBB : WhiteKnightPositionBB;
        case 2: // bishop
            return colour ? BlackBishopPositionBB : WhiteBishopPositionBB;
        case 3: // rook
            return colour ? BlackRookPositionBB : WhiteRookPositionBB;
        case 4: // queen
            return colour ? BlackQueenPositionBB : WhiteQueenPositionBB;
        case 5: // king
            if (endgame) {
                return colour ? BlackKingEndPositionBB : WhiteKingEndPositionBB;
            } else {
                return colour ? BlackKingPositionBB : WhiteKingPositionBB;
            }
        default:
            return nullptr;
    }
}