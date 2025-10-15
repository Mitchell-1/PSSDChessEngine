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

        MoveList(Move MoveList[]);
        const Move* begin() const {return moveList;}
        const Move* end() const {return last;}
        size_t size() const {return last - moveList;}
        bool contains(Move move) const { return std::find(begin(), end(), move) != end(); }
        void printNestedMoves(){
            printf("Colour   Piece   Start    End  Capture Promotion En-Passantable\n");
            for (int i = 0; i < this->size();i++){
                printf(" %d  -   ", i);
                myEngine::printMove(moveList[i]);
                //getchar();
                //exit(0);
            }
            printf("Done Printing Moves \n");
            //exit(0);
        }
    };


#endif