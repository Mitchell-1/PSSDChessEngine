#ifndef move_h
#define move_h
#include <iostream>
#include <cstdint>

using Bitboard = uint64_t;
using Square = int;

// Bit 0-5 is square from
// Bit 6-11 is square to
// Bit 12-13 is promotion type
// Bit 14-15 is special flag 01 = promotion, *10 = king castle, 11 = queen castle
// Bit 16-18 is for piece moved
// Bit 19-21 is for piece captured
// Bit 22 is for move colour
// Bit 23 is for passantable move

// Bit 24-27 is score for breaking castle memory
// 0001 broke black king castle
// 0010 broke black queen castle
// 0100 broke white king castle
// 1000 broke white queen castle

class Move {
    protected:
        uint32_t data;
    public:
        int value;
        Move() : data(0) {}
        Move(uint32_t d): data(d) {}
        Move(Square from, Square to, int promo, int flag, int type, int capture, bool colour, bool passantable){
            value = 0;
            if (capture != 7) {
                value += 100 * (1 + capture - (1+type));
            }
            value += flag == 1 ? promo * 100 : 0;
            value += passantable ? 50 : 0;
            data = (passantable << 23) + (colour << 22) + (capture << 19) + (type << 16) + (flag << 14) + (promo << 12)+(to << 6)+ from;
        }
        void updateData(int newData, int offset, int size) {
            int range = ((Bitboard(1) << size )-1) << offset;
            data &= ~range;
            data |= (newData << offset);
        }
        void orData(int newData, int offset) {
            data |= (newData << offset);
        }
        Square fromSquare() const {
            return Square(data & 0x3F);
        };
        Square toSquare() const{
            return Square((data >> 6) & 0x3F);
        };
        Square promotion() const {
            return ((data >> 12) & 3);
        };
        Square special() const {
            return ((data >> 14) & 3);
        };
        Square type() const {
            return ((data >> 16) & 7);
        }
        Square capture() const {
            return ((data >> 19) & 7);
        }
        bool colour() const {
            return ((data >> 22) & 1);
        }
        bool passantable() const {
            return ((data >> 23) & 1);
        }
        int brokeCastle() const {
            return ((data >> 24) & 15);
        }
        uint32_t raw() const { return data; }
        //operator =(const Move& move2) {return move}
        bool operator >(const Move& move2) const {return this->value > move2.value;};
        bool operator <(const Move& move2) const {return this->value < move2.value;};
};


#endif