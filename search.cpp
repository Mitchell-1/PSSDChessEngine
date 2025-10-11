#include "search.h"


// Implement move selection here. This is a placeholder so that the function can work for now.
Move Search::FindBestMove(Game &game, int depth) {
            // Placeholder for search algorithm
            Move bestMove;
            MoveList moveList = MoveList<LEGAL>(game);
            if (moveList.size() > 0) {
                bestMove = *(moveList.begin());
            } else {
                bestMove = Move();
            }
            return bestMove;
        }