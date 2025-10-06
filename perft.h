#ifndef perft_h
#define perft_h

#include <iostream>
#include "moveList.h"
#include <chrono>

//Runs a perft test to return the number of possible nodes at a given depth.
template<bool root>
uint64_t perft(Game& position, int depth) {
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t count = 0, nodes = 0;
    const bool leaf = (depth == 2);
    for (Move m: MoveList<LEGAL>(position)) {
        if (root && depth <= 1) {
            if (position.makeMove(m)){
                continue;
            }
            count = 1, nodes++;
            position.unMakeMove(m);
        } else {
            if (position.makeMove(m)){
                continue;
            }
            if (leaf) {
                count = 0;
                for (Move m2: MoveList<LEGAL>(position)) {
                    if (!(position.isPosOk(m2))){
                        
                        continue;
                    }
                    nodes++;
                }  
            } else {
                count = perft<false>(position, depth-1);
                nodes += count;
            }
            
            position.unMakeMove(m);
        }
        if (root) {
            printMove(m);
            std::cout << count << std::endl;
        }
    }
    if (root) {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        printf("total move generation time: %llu \n", duration);
    }
    return nodes;
}



#endif