#include "search.h"
#include "evaluate.h"
#include "transpositionTable.h"

namespace search_impl {
    // return pair<score, move>
    struct ScoredMove{
        int score;
        Move move;
    };

    TranspositionTable TTable = TranspositionTable(0);
    int numEntries;

    static uint64_t nodesSearched = 0;
    static uint64_t cutoffs = 0;
    static uint64_t hashHits = 0;
    int maxExtension = 8;

    int getMoveExtension(Game &game, const Move &move) {
        if (game.inCheck(game.turn)) {
            return 1;
        }
        // extend if pawn close to promoting
        if (move.colour()) {
            if (move.type() == 0 && (move.toSquare() /8) == 1) {
                return 2;
            }
        } else {
            if (move.type() == 0 && (move.toSquare() /8) == 6) {
                return 2;
            }
        }
        return 0;
    }

    ScoredMove quiescence(Game &game, int alpha, int beta, int extensions, int depthFromRoot) { 
        int startEval = Evaluate::evaluateBoard(game, 0);

        if (extensions >= maxExtension) {
            nodesSearched++;
            return ScoredMove{startEval, 0};
        }

        int bestScore = startEval;
        MoveList<CAPTURES> moves(game);
        if (moves.size() == 0) {
            nodesSearched++;
            return ScoredMove{bestScore, 0}; // no captures, return evaluation
        }
        int score;
        for (const Move *it = moves.begin(); it != moves.end(); ++it){
            Move m = *it;
            game.makeMove(m);
            uint64_t tempHash = game.hash;
            game.updateHash(m);
            int Entry = TTable.lookupHash(game.hash, extensions, alpha, beta);
            if (Entry != TTable.failedLookup) {
                hashHits++;
                score = Entry;
            } else {
                score = quiescence(game, alpha, beta, extensions + 1, depthFromRoot + 1).score;
                TTable.storeHash(game.hash, extensions, TTable.exactScore, score);
            }
            game.unMakeMove(m);
            game.hash = tempHash;
            if (game.turn) {
                if (score < bestScore) {
                    bestScore = score;
                }
                beta = std::min(beta, bestScore);
            } else {
                if (score > bestScore) {
                    bestScore = score;
                }
                alpha = std::max(alpha, bestScore);
            }
            if (alpha >= beta) {
                cutoffs++;
                break; // cut-off
            }
    }
        return {bestScore, 0};
    }
    
    ScoredMove minimax(Game &game, int depth, int alpha, int beta, int extensions, int depthFromRoot) {
        // threefold repetition detection
        size_t histSize = game.moveHistory.size() - 1;
        if (histSize >= 9) {
            if (game.hash == game.moveHistory[histSize - 5] && game.hash == game.moveHistory[histSize - 9]) {
                nodesSearched++;
                return ScoredMove{0, 0};
            
            }
        }
        
        // at depth 0 evaluate and return
        if (depth == 0){
            if (extensions <= maxExtension) {
                return quiescence(game, alpha, beta, extensions + 1, depthFromRoot + 1);
            }
            nodesSearched++;
            return ScoredMove{Evaluate::evaluateBoard(game, 0), 0}; //Evaluation and Null move
        }
        MoveList<LEGAL> moves(game);
        std::stable_sort(moves.begin(), moves.end(), std::greater<Move>());
        size_t n = moves.size();
        if (n == 0){
            // if no legal moves, return evaluation
            return ScoredMove{Evaluate::evaluateBoard(game, 0, 0), 0}; //Evaluation with optional tag and Null move
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
        ScoredMove child;
        int evalBound = TTable.upperBound;
        for (const Move *it = moves.begin(); it != moves.end(); ++it){
            Move m = *it;
            game.makeMove(m);
            int tempHash = game.hash;
            game.updateHash(m);
            int Entry = TTable.lookupHash(game.hash, depth, alpha, beta);
            if (Entry != TTable.failedLookup) {
                hashHits++;
                child.score = Entry;
            } else {
                game.moveHistory.push_back(game.hash);
                int extension = getMoveExtension(game, m);
                extension = extensions + extension <= maxExtension ? extension : 0;
                child = minimax(game,depth - 1 + extension, alpha, beta, extension + extensions, depthFromRoot + 1);
                game.moveHistory.pop_back();
            }
            // revert
            game.unMakeMove(m);
            game.hash = tempHash;
            if (maximise) {
                if (child.score > best.score){
                    best.score = child.score;
                    best.move = m;
                    evalBound = TTable.exactScore;
                }
                alpha = std::max(alpha, best.score); // update alpha
            } else {
                if (child.score < best.score){
                    best.score = child.score;
                    best.move = m;
                }
                beta = std::min(beta, best.score); // update beta
            }
            // alpha-beta cutoff
            if (alpha >= beta){
                TTable.storeHash(game.hash, depth + 1, TTable.lowerBound, alpha);
                cutoffs++;
                break;
            }
            
        }
        TTable.storeHash(game.hash, depth + 1, evalBound, best.score);
        return best;
    }

    // helpers to access instrumentation
    void resetStats(){ 
        nodesSearched = 0; 
        cutoffs = 0; 
        hashHits = 0;
    }
    void getStats(uint64_t &nodes, uint64_t &co, uint64_t &hash){ 
        nodes = nodesSearched; 
        co = cutoffs; 
        hash = hashHits;
    }
};

Search::Search(int verbose) : verbosity(verbose) {
    int sizeMB = 64; // default 64 MB transposition table
    int sizeBytes = sizeMB * 1024 * 1024;
    int numEntries = sizeBytes / sizeof(uint64_t);
    search_impl::numEntries = numEntries;
    search_impl::TTable = TranspositionTable(numEntries);
}

Move Search::FindBestMove(Game &game, int depthMax) {
    if (depthMax <= 0) {
        return Move();
    }
    search_impl::maxExtension = 3; // set max extension to depth to avoid infinite extensions\

    uint64_t nodes, co, hashHits;
    auto start = std::chrono::high_resolution_clock::now();

    int alpha = INT32_MIN;
    int beta = INT32_MAX;
    search_impl::resetStats();
    
    MoveList<LEGAL> moves(game);
    if (moves.size() == 0) {
        return Move(); // no legal moves, return null move
    }

    
    search_impl::ScoredMove best;
    int depthMin = depthMax > 2 ? 2 : depthMax;
    for (int depth = depthMin; depth <= depthMax; depth++){
        std::cout << "Searching at depth " << depth << "...\n";
        std::stable_sort(moves.begin(), moves.end(), std::greater<Move>());
        best.move = *moves.begin();
        if (game.turn){
            best.score = INT32_MAX;
        } else {
            best.score = INT32_MIN;
        }
        for (Move *it = moves.begin(); it != moves.end(); ++it){
                Move m = *it;
                game.makeMove(m);
                int tempHash = game.hash;
                game.updateHash(m);
                game.moveHistory.push_back(game.hash);
                search_impl::ScoredMove child = search_impl::minimax(game,depth - 1, alpha, beta, 0, 1);
                game.unMakeMove(m);
                game.hash = tempHash;
                game.moveHistory.pop_back();

                printMove(m);
                std::cout << " Score: " << child.score << std::endl;

                if (game.turn) {
                    if (child.score < best.score){
                        best.score = child.score;
                        best.move = m;
                    }
                    beta = std::min(beta, best.score);
                } else {
                    if (child.score > best.score){
                        best.score = child.score;
                        best.move = m;
                    }
                    alpha = std::max(alpha, best.score);
                }
            it->value = child.score; // update move ordering value
        }
        if (game.turn) {
            if (best.score == INT32_MIN) {
                break; // checkmate found
            }
        } else {
            if (best.score == INT32_MAX) {
                break; // checkmate found
            }
        }
    }
    

    if (verbosity > 0) {
        search_impl::getStats(nodes, co, hashHits);
        std::cerr << "Search nodes: " << nodes << " Prunes: " << co << " Hash hits: " << hashHits << std::endl;
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