#ifndef perft_h
#define perft_h

#include <iostream>
#include "moveList.h"
#include <chrono>

//Runs a perft test to return the number of possible nodes at a given depth.
template<bool root>
uint64_t perft(Game& position, int depth, int verbosity=0) {
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t count = 0, nodes = 0;
    const bool leaf = (depth == 2);
    // Iterate through all legal moves in the current position
    for (Move m: MoveList<LEGAL>(position)) {
        // if the initial depth passed is 1, each move is a leaf node
        // so no recursion is needed. 
        if (root && depth <= 1) {
            count = 1, nodes++;
        } else {
            // Position updating and recursive perft call
            // to count nodes at the desired depth.
            position.makeMove(m);
            // If there is a depth of 2 left, batch count the legal moves
            // to save on recursive calls.
            if (leaf) {
                count = MoveList<LEGAL>(position).size();
                nodes += count;
            } else {
                // Recursive perft call for depths greater than 2
                count = perft<false>(position, depth-1);
                nodes += count;
            }
            position.unMakeMove(m);
        }
        // Output the move and its corresponding node count if 
        // verbosity is set and this is the root call.
        if (root && verbosity > 0) {
            printMove(m);
            std::cout << count << std::endl;
        }
    }
    if (root) {
        // Final output of total nodes and time taken for the perft test.
        // Also outputs a formatted nodes per second for performance measurement.
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        printf("total move generation time: %llu:%02llu:%03llu \n", duration/60000,(duration/1000)%60, duration%1000);
        printf("total nodes: %llu \n", nodes);
        if (duration.count() == 0) {
            duration = std::chrono::milliseconds(1);
        }
        std::string nps = std::to_string(nodes*1000/duration.count());
        for (int i = nps.length() - 3; i > 0; i -= 3) {
            nps.insert(i, ",");
        }
        printf("nodes per second: %s \n", nps.c_str());
    }
    return nodes;
}



#endif