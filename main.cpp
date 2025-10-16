#include "moveGen.h"
#include "preMoveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"
#include "search.h"
#include "evaluate.h"
#include <iostream>
#include <string>

void perftSuite(Game & game) {
    struct PerftResult {
        int depth;
        uint64_t nodes;
    };
    PerftResult startPosResults[] = {
        {1, 20},
        {2, 400},
        {3, 8902},
        {4, 197281},
        {5, 4865609},
        {6, 119060324}
    };

    PerftResult testPosResults[] = {
        {1, 48},
        {2, 2039},
        {3, 97862},
        {4, 4085603},
        {5, 193690690}
    };

    for (const auto& result : startPosResults) {
        std::cout << "Starting position perft to depth " << result.depth << std::endl;
        uint64_t nodes = perft<true>(game, result.depth);
        if (nodes == result.nodes) {
            std::cout << "Depth " << result.depth << ": Passed (" << nodes << " nodes)" << std::endl;
        } else {
            std::cout << "Depth " << result.depth << ": Failed (Expected " << result.nodes << ", Got " << nodes << ")" << std::endl;
        }
        std::cout << std::endl;
    }

    game = Game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    for (const auto& result : testPosResults) {
        std::cout << "Test position perft to depth " << result.depth << std::endl;
        uint64_t nodes = perft<true>(game, result.depth);
        if (nodes == result.nodes) {
            std::cout << "Depth " << result.depth << ": Passed (" << nodes << " nodes)" << std::endl;
        } else {
            std::cout << "Depth " << result.depth << ": Failed (Expected " << result.nodes << ", Got " << nodes << ")" << std::endl;
        }
        std::cout << std::endl;
    }

}

void UCI_Listen(Game & game, int verbosity) {
    std::string input;
    Search Searcher(verbosity);
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
                uint64_t nodes = perft<true>(game, depth, 1);
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
        } else if (input.substr(0, 11) == "perftSuite") {
            perftSuite(game);
        } else if (input.substr(0, 4) == "test") {
            int eval = Evaluate::evaluateBoard(game, 1);
            std::cout << "Evaluation of current position: " << eval << std::endl;

        }
            
    }
};

int main() {
    genPossibleBishopMoves();
    genPossibleRookMoves();
    std::string startingFen;
    startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string testFen = "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2";
    Game game(startingFen);
    int verbosity = 1;
    UCI_Listen(game, verbosity);
}




