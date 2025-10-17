#ifndef moves_h
#define moves_h
#include "move.h"
#include "game.h"
#include "moveGen.h"
#include <iostream>
#include <algorithm>
#include <cstddef>

template <genType Type>
struct MoveList {
    private:
        Move tempMoveList[256], *tempLast;
        Move moveList[256], *last;
    public:
        MoveList();
        explicit MoveList(Game& position) : 
            // Generate all moves of the specified type into a temporary list
            // then test for legality before adding to the final move list.
            tempLast(generate<Type>(position, tempMoveList)) {
                Move* move;
                int counter = 0;
                for (move = tempMoveList; move != tempLast; ++move) {
                    if (position.isMoveOk(*move)){
                        continue;
                    }
                    moveList[counter++] = *move;
                }
                last = &moveList[counter];
            }
        // Helper functions for iterating over the move list and sorting.
        Move * begin() {return moveList;}
        Move * end() {return last;}
        const Move* begin() const {return moveList;}
        const Move* end() const {return last;}
        size_t size() const {return last - moveList;}
        bool contains(Move move) const { return std::find(begin(), end(), move) != end(); }
        void printNestedMoves(){
            printf("Colour   Piece   Start    End  Capture Promotion En-Passantable\n");
            for (int i = 0; i < this->size();i++){
                printf(" %d  -   ", i);
                myEngine::printMove(moveList[i]);
            }
            printf("Done Printing Moves \n");
        }
    };


#endif