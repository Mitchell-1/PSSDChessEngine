#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <cstdint>


struct TTEntry {
    uint64_t hash;
    int depth;
    int flag;
    int score;
    TTEntry(uint64_t h, int d, int f, int s) : hash(h), depth(d), flag(f), score(s) {};
    TTEntry() : hash(0), depth(0), flag(0), score(0) {};
};

class TranspositionTable {
public:
    bool enabled = true;
    const int failedLookup = -2;
    const int exactScore = -1;
    const int lowerBound = -3;
    const int upperBound = -4;

    TTEntry *table;
    uint64_t size;

    TranspositionTable(uint64_t size) : size(size) {
        table = new TTEntry[size];
    }

    TTEntry getEntry(uint64_t hash) {
        return table[hash % size];
    }

    int lookupHash(uint64_t hash, int depth, int alpha, int beta) {
        if (!enabled) {
            return failedLookup;
        }
        TTEntry entry = getEntry(hash);
        if (entry.hash == hash) {
            if (entry.depth >= depth) {
                if (entry.flag == exactScore) {
                    return entry.score;
                } else if (entry.flag == lowerBound && entry.score > alpha) {
                    alpha = entry.score;
                } else if (entry.flag == upperBound && entry.score < beta) {
                    beta = entry.score;
                }
                if (alpha >= beta) {
                    return entry.score;
                }
            }
        }
        return failedLookup;
    }

    void storeHash(uint64_t hash, int depth, int flag, int score) {
        TTEntry entry = TTEntry(hash, depth, flag, score);
        table[hash % size] = entry;
    }

    void operator=(const TranspositionTable &other) {
        if (this != &other) {
            delete[] table;
            size = other.size;
            table = new TTEntry[size];
        }
    }

    ~TranspositionTable() {
        delete[] table;
    }

};


#endif