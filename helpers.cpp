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


int myEngine::squareToInt(std::string square){
    std::string files1 = "abcdefgh";
    char file = square[0];
    int fileNum = files1.find(file);
    int rank = square[1] - '0' - 1;
    return rank*8 + fileNum;
}

void myEngine::printMove(Move move){
    const char * colour = move.colour() ? "Black  " : "White  ";
    switch (move.type())
    {
    case 0:
        printf("%s  Pawn      %s  ->  %s     %d      %d       %d           %d\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture(), 0, (move.special()==1)*(move.promotion()+1), move.passantable());
        break;
    case 2:
        printf("%s  Bishop    %s  ->  %s     %d      %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture(), 0 );
        break;
    case 1:
        printf("%s  Knight    %s  ->  %s     %d      %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture(), 0 );
        break;
    case 3:
        printf("%s  Rook      %s  ->  %s     %d      %d       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture(), 0 );
        break;
    case 4:
        printf("%s  Queen     %s  ->  %s     %d      %d       N           N\n" , colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture(), 0 );
        break;
    case 5:
        printf("%s  King      %s  ->  %s     %d      N       N           N\n", colour, STC[move.fromSquare()], STC[move.toSquare()], move.capture());
        break;
    default:
        break;
    }
}