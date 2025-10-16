#include "search.h"
#include "evaluate.h"

namespace search_impl {
    // return pair<score, move>
    struct ScoredMove{
        int score;
        Move move;
    };

    static uint64_t nodesSearched = 0;
    static uint64_t cutoffs = 0;

    ScoredMove minimax(Game &game, int depth, int alpha, int beta) {
        // at depth 0 evaluate and return
        nodesSearched++;
        if (depth == 0){
            return ScoredMove{Evaluate::evaluateBoard(game), 0}; //Evaluation and Null move
        }

        MoveList<LEGAL> moves(game);
        size_t n = moves.size();
        if (n == 0){
            // if no legal moves, return evaluation
            return ScoredMove{Evaluate::evaluateBoard(game, 0), 0}; //Evaluation with optional tag and Null move
        }

        bool sideToMove = game.turn; // 0 = white, 1 = black

        // for white we want to maximise, for black we minimise
        bool maximise = (sideToMove == 0);

        ScoredMove best;
        best.move = *moves.begin();
        if (maximise){
            best.score = INT32_MIN;
        } else {
            best.score = INT32_MAX;
        }

        for (const Move *it = moves.begin(); it != moves.end(); ++it){
            Move m = *it;
            game.makeMove(m);

            ScoredMove child = minimax(game, depth - 1, alpha, beta);

            // revert
            game.unMakeMove(m);
            if (maximise){
                if (child.score > best.score){
                    best.score = child.score;
                    best.move = m;
                }
                if (child.score > alpha) alpha = child.score; // update alpha
            }else{
                if (child.score < best.score){
                    best.score = child.score;
                    best.move = m;
                }
                if (child.score < beta) beta = child.score; // update beta
            }
            // alpha-beta cutoff
            if (alpha >= beta){
                cutoffs++;
                break;
            }
        }

        return best;
    }
    
    // helpers to access instrumentation
    void resetStats(){ 
        nodesSearched = 0; 
        cutoffs = 0; 
    }
    void getStats(uint64_t &nodes, uint64_t &co){ 
        nodes = nodesSearched; 
        co = cutoffs; 
    }
};

Move Search::FindBestMove(Game &game, int depth) {
    if (depth <= 0) {
        return Move();
    }
    int alpha = INT32_MIN;
    int beta = INT32_MAX;

    search_impl::resetStats();
    search_impl::ScoredMove best = search_impl::minimax(game, depth, alpha, beta);

    uint64_t nodes, co;
    search_impl::getStats(nodes, co);
    std::cerr << "Search nodes: " << nodes << " cutoffs: " << co << std::endl;
    return best.move;
};