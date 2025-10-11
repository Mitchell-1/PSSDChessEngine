#ifndef SEARCH_H
#define SEARCH_H

#include "moveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"
#include <iostream>
#include <string>

class Search {
    public:
        Search() {};
        Move FindBestMove(Game &game, int depth);
};


#endif