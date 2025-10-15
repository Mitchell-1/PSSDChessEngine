#include "search.h"
#include "evaluate.h"



namespace search_impl {
    
    // return pair<score, move>
    struct ScoredMove{
        int score;
        Move move;
    };
    uint64_t nodesSearched = 0;
    ScoredMove minimax(Game &game, int depth) {
        // at depth 0 evaluate and return

        if (game.moveHistory.size() >= 9) { 
            int currHash = game.hash;
            int hash1 = game.moveHistory[game.moveHistory.size() - 5];
            int hash2 = game.moveHistory[game.moveHistory.size() - 9];
            if (currHash == hash1 && currHash == hash2) {
                nodesSearched++;
                return ScoredMove{0, 0}; // threefold repetition draw
            }
        }

        if (depth == 0){
            nodesSearched++;
            return ScoredMove{Evaluate::evaluateBoard(game), 0}; //Evaluation and Null move
        }

        MoveList<LEGAL> moves(game);
        size_t n = moves.size();
        if (n == 0){
            // if no legal moves, return evaluation.
            // Evaluation checks for checkmate or stalemate
            // via the optional moves parameter. 
            nodesSearched++;
            return ScoredMove{Evaluate::evaluateBoard(game, 0), 0}; //Evaluation with optional tag and Null move
        }
        

        bool sideToMove = game.turn; // 0 = white, 1 = black

        // for white we want to maximise, for black we minimise
        bool maximise = (sideToMove == 0);

        ScoredMove best;
        best.move = *moves.begin();
        if (maximise){
            best.score = INT32_MIN;
        }else{
            best.score = INT32_MAX;
        }
        for (const Move *it = moves.begin(); it != moves.end(); ++it){
            Move m = *it;
            game.makeMove(m);
            int tempHash = game.hash;
            game.updateHash(m);
            game.moveHistory.push_back(game.hash);
            ScoredMove child = minimax(game,depth - 1);
            // revert
            game.unMakeMove(m);
            game.hash = tempHash;
            game.moveHistory.pop_back();

            if (maximise) {
                if (child.score > best.score){
                    best.score = child.score;
                    best.move = m;
                }
            } else {
                if (child.score < best.score){
                    best.score = child.score;
                    best.move = m;
                }
            }
        }

        return best;
    }
};

Move Search::FindBestMove(Game &game, int depth) {
    if (depth <= 0) {
        return Move();
    }
    search_impl::nodesSearched = 0;
    search_impl::ScoredMove best = search_impl::minimax(game, depth);
    if (verbosity > 0) {
        std::cout << " Score: " << best.score  << " Nodes Searched: " << search_impl::nodesSearched << std::endl;
    }
    
    return best.move;
};