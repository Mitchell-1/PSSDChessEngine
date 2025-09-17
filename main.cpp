#include "moveGen.h"
#include "preMoveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"


int main() {
    Bitboard test = 1;
    genPossibleBishopMoves();
    genPossibleRookMoves();
    std::string startingFen;
    int depth = 6;

    startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    Game game(startingFen);
    //game.mainBoard.printBoard();
    std::cout << "total nodes: " <<  perft<true>(game, 7) << std::endl;
    


    

}