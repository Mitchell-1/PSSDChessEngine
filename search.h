#ifndef SEARCH_H
#define SEARCH_H

#include "moveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"
#include <iostream>
#include <string>
#include <unordered_map>

class Search {
    public:
        int verbosity = 0;
        Search(int verbose);
        Move FindBestMove(Game &game, int depth);
};


#endif