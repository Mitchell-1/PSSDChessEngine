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
            position.makeMove(m);
            count = 1, nodes++;
            position.unMakeMove(m);
        } else {
            position.makeMove(m);
            if (leaf) {
                count = 0;
                for (Move m2: MoveList<LEGAL>(position)) {
                    if (!(position.isPosOk(m2))){
                        
                        continue;
                    }
                    nodes++;
                    count++;
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
        printf("total move generation time: %llu:%02llu:%03llu \n", duration/60000,(duration/1000)%60, duration%1000);
        printf("total nodes: %llu \n", nodes);
        std::string nps = std::to_string(nodes*1000/duration.count());
        for (int i = nps.length() - 3; i > 0; i -= 3) {
            nps.insert(i, ",");
        }
        printf("nodes per second: %s \n", nps.c_str());
    }
    return nodes;
}



#endif