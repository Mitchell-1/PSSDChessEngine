#ifndef EVALUATE_H
#define EVALUATE_H

#include "moveGen.h"
#include "moveList.h"
#include "game.h"
#include "perft.h"
#include <iostream>
#include <string>

class Evaluate {
    public:
        Evaluate() {};
        int evaluateBoard(const Game &game);
};

#endif