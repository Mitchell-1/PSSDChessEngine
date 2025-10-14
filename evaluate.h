#ifndef EVALUATE_H
#define EVALUATE_H

#include "moveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"
#include "helpers.h"
#include <iostream>
#include <string>

class Evaluate {
    public:
        Evaluate() {};
        static int evaluateBoard(const Game &game, int moves = 1);
};

#endif