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
            int tempHash = game.hash;
            game.updateHash(m);
            game.moveHistory.push_back(game.hash);

            ScoredMove child = minimax(game,depth - 1, alpha, beta);

            // revert
            game.unMakeMove(m);
            game.hash = tempHash;
            game.moveHistory.pop_back();

            if (maximise) {
                if (child.score > best.score){
                    best.score = child.score;
                    best.move = m;
                }
                if (child.score > alpha) alpha = child.score; // update alpha
            } else {
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
    uint64_t nodes, co;
    auto start = std::chrono::high_resolution_clock::now();

    int alpha = INT32_MIN;
    int beta = INT32_MAX;

    search_impl::resetStats();
    search_impl::ScoredMove best = search_impl::minimax(game, depth, alpha, beta);

    
    if (verbosity > 0) {
        
        search_impl::getStats(nodes, co);
        std::cerr << "Search nodes: " << nodes << " Prunes: " << co << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (duration.count() == 0) {
            duration = std::chrono::milliseconds(1);
        }
        printf("total move generation time: %llu:%02llu:%03llu \n", duration/60000,(duration/1000)%60, duration%1000);
        std::string nps = std::to_string(nodes*1000/duration.count());
        for (int i = nps.length() - 3; i > 0; i -= 3) {
            nps.insert(i, ",");
        }
        printf("nodes per second: %s \n", nps.c_str());
        std::cout << "Score: " << best.score << std::endl;
    }
    
    return best.move;
};