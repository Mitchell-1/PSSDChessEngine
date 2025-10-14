#ifndef moveGen_h
#define moveGen_h
#include <iostream>
#include "helpers.h"
#include "boardHelpers.h"
#include "move.h"
#include "game.h"
#include "preMoveGen.h"

enum genType {
    CAPTURES,
    QUIETS,
    EVASIONS,
    NON_EVASIONS,
    LEGAL
};

template<genType>
Move * generate(Game& position, Move * moveList);

#endif