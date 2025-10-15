#include "moveGen.h"
#include "preMoveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"
#include "search.h"
#include <iostream>
#include <string>


void UCI_Listen(Game & game) {
    std::string input;
    Search Searcher(0);
    while (true) {
        std::getline(std::cin, input);
        if (input == "quit") {
            break;
        } else if (input == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (input.substr(0, 8) == "position") {
            if (input.find("startpos") != std::string::npos) {
                game = Game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            } else if (input.find("fen") != std::string::npos) {
                std::string fen = input.substr(input.find("fen") + 4);
                game = Game(fen);
            }
        } else if (input.substr(0, 2) == "go") {
            int depth = 6; // default depth
            if (input.find("depth") != std::string::npos) {
                depth = std::stoi(input.substr(input.find("depth") + 6));
            }

            if (input.find("perft") != std::string::npos) {
                depth = std::stoi(input.substr(input.find("perft") + 6));
                printf("Format:\nColour   Piece   Start    End  Capture Promotion En-Passantable\nNodes\n\n");
                uint64_t nodes = perft<true>(game, depth);
                continue;
            }

            // Standard notation features a lot more params to move gen that will be handeled here.
            // But for now just do the algorithm and we can worry about adding more features later.


            Move bestMove = Searcher.FindBestMove(game, depth);
            

            if (bestMove.raw() != 0) {
                //std::cout << myEngine::STC[bestMove.fromSquare()]  << myEngine::STC[bestMove.toSquare()] << std::endl;
                printMove(bestMove);
                game.makeMove(bestMove);
                game.updateHash(bestMove);
                game.moveHistory.push_back(game.hash);
            } else {
                std::cout << "bestmove (none)" << std::endl;
                continue;
            }
        } else if (input.substr(0, 8) == "makeMove") {
            std::string moveStr = input.substr(9);
            game.recieveMove(moveStr);
        }
    }
};

int main() {
    Bitboard test = 1;
    genPossibleBishopMoves();
    genPossibleRookMoves();
    std::string startingFen;
    int depth = 6;

    startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    Game game(startingFen);
    //printBitBoard(game.mainBoard);
    //std::cout << "total nodes: " <<  perft<true>(game, 7) << std::endl;
    
    UCI_Listen(game);


    

}


