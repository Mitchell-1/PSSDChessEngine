#include "search.h"
#include "evaluate.h"

namespace search_impl {
    // return pair<score, move>
    struct ScoredMove{
        int score;
        Move move;
    };

    ScoredMove minimax(Game &game, int depth) {
        Evaluate evaluator;
        // at depth 0 evaluate and return
        if (depth == 0){
            return ScoredMove{evaluator.evaluateBoard(game), Move()};
        }

        MoveList<LEGAL> moves(game);
        size_t n = moves.size();
        if (n == 0){
            // if no legal moves, return evaluation 
            // mate/stalemate handling can be added later by evaluation function
            return ScoredMove{evaluator.evaluateBoard(game), Move()};
        }

        bool sideToMove = game.turn; // 0 = white, 1 = black

        // for white we want to maximise, for black we minimise
        bool maximise = (sideToMove == 0);

        ScoredMove best;
        if (maximise){
            best.score = INT32_MIN;
        }else{
            best.score = INT32_MAX;
        }

        for (const Move *it = moves.begin(); it != moves.end(); ++it){
            Move m = *it;
            if (game.makeMove(m)){
                // king is in check, move is undone by makeMove already so we can just skip this move
                continue;
            }

            ScoredMove child = minimax(game,depth - 1);

            // revert
            game.unMakeMove(m);

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
}

Move Search::FindBestMove(Game &game, int depth) {
    if (depth <= 0) {
        return Move();
    }
    search_impl::ScoredMove best = search_impl::minimax(game, depth);
    return best.move;
}