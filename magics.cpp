#include "magics.h"
unsigned int state = 1804289383;

unsigned int getRandomNumber(){
    unsigned int number = state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    state = number;
    return number;
}

uint64_t getRandomU64(){
    uint64_t n1, n2, n3, n4;
    n1 = uint64_t(getRandomNumber()) & 0xFFFF;
    n2 = uint64_t(getRandomNumber()) & 0xFFFF;
    n3 = uint64_t(getRandomNumber()) & 0xFFFF;
    n4 = uint64_t(getRandomNumber()) & 0xFFFF;
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

int getBitCount(uint64_t board){
    int count = 0;
    while (board){
        count ++;
        board &= board -1;
    }
    return count;
}


uint64_t rookMagics[64] ={
0x80001060854000ULL, 
0x4840400010002000ULL,
0x180200081100008ULL, 
0x2080100004820800ULL, 
0x8200041002000920ULL, 
0x100040018066900ULL, 
0x400440088011002ULL, 
0x100004081000022ULL, 
0x100802080004008ULL, 
0x800402010004000ULL,
0x4002001200402084ULL, 
0x5080800800801000ULL, 
0x8414800800040280ULL, 
0x12001008420004ULL,
0x42003200840148ULL, 
0x200032480440aULL, 
0x228002400080ULL, 
0x30004000200050ULL, 
0x4002020010884020ULL,
0x888008010000880ULL, 
0x132828004000800ULL, 
0x840818012000400ULL, 
0x4500240008619002ULL, 
0x400020013004684ULL, 
0x80004040002000ULL, 
0x8800200480400380ULL,
0x8080200100410010ULL,
0x90100180080081ULL, 
0x80100100500ULL, 
0x44000480020080ULL, 
0x201211400021008ULL, 
0x1288210600044084ULL, 
0x300401020800080ULL, 
0x4142010042003080ULL,
0x20811002802001ULL,
0x518020010100100ULL, 
0x100801000500ULL, 
0x56002004040010ULL, 
0x810025004004108ULL,
0xc06004082000401ULL, 
0x4180804000208002ULL,
0x89200040008a8020ULL, 
0x8200011010040ULL, 
0x100080010008080ULL, 
0x50008010011ULL, 
0x1006001810060004ULL, 
0x480218015004008aULL, 
0x5101284400820009ULL, 
0x800211480084100ULL, 
0x112400688210100ULL,
0x401108200100ULL, 
0xc000100080080480ULL, 
0x4400080004008080ULL,
0x202000204008080ULL, 
0x402080210213400ULL,
0x80c0120984200ULL, 
0x6000820410214302ULL, 
0x45011420400281ULL, 
0x404290020029141ULL, 
0x800140821700101ULL, 
0x103000210080005ULL, 
0x448100240008820dULL, 
0x51000200d1100804ULL,
0x410008024010042ULL
};
uint64_t RookAttackMasks[64][4096];
uint64_t bishopMagics[64] = {
0x4011001010101ULL, 
0x10901210484080ULL, 
0x1064240182128000ULL, 
0x48204043800104ULL, 
0xc001104042800000ULL, 
0x8202082405000204ULL, 
0x81010820840400ULL, 
0x8a820802010480ULL, 
0x8444100a180120ULL, 
0xa000420806488608ULL, 
0x9484810c010020ULL, 
0x40044402800000ULL, 
0x820040420004200ULL, 
0x1100020202200402ULL, 
0x801a408610102580ULL, 
0x8100004c04040223ULL, 
0x40100803040402ULL, 
0x20850404089200ULL, 
0x18005380810200ULL, 
0x80800022022401aULL,
0x94005a11040001ULL,
0x4810808010804ULL, 
0x2024000a06290400ULL,
0x1405080200808404ULL,
0x20080184080880ULL,
0x801300004100242ULL,
0x6014864188080100ULL,
0x8100808020820ULL, 
0xb0040000802110ULL,
0x420522000ac80410ULL,
0x2608102009002ULL,
0x2084000840880ULL,
0x4802082000400300ULL, 
0x4022000080100ULL,
0x8029080200010401ULL,
0xc08480080200ULL, 
0x44040401001100ULL,
0x812880244420101ULL,
0x41090210610800ULL,
0x8801104200010100ULL,
0x224220884014003ULL,
0x880410600208ULL,
0x10a0240840d000ULL, 
0xa1002018003105ULL,
0x810080100430400ULL,
0x408101090880200ULL,
0x3002980109040400ULL,
0x204008202000c44ULL,
0x2011210802400014ULL,
0x2300308210100400ULL,
0x8000004108210e05ULL,
0x8001084040a80ULL,
0x2021301002020400ULL,
0x4280100330250410ULL,
0x528a0c0360c45020ULL,
0x4100c89091000ULL,
0x408120a414044000ULL,
0x800c08080882000ULL, 
0x2014000180482860ULL,
0xca424142a0800ULL,
0x8102400ULL,
0x4040000a10810200ULL,
0x40042144041080ULL,
0x4401001120e2040ULL
};
uint64_t bishopAttackMasks[64][512];

uint64_t * genBishopMask() {
    static uint64_t attackMasks[64] = {0};
    uint64_t one = 1;
    for (int square = 0; square<64; square++){
        int r, f;
        int tr = square / 8;
        int tf = square % 8;
        for (r=tr + 1, f = tf+1; r <= 6 && f<=6; r++, f++){
            attackMasks[square] |= (one << (r*8 + f));
        }
        for (r=tr-1, f = tf-1; r >=1 && f>=1; r-- , f-- ){
            attackMasks[square] |= (one << (r*8 + f));
        }
        for (r=tr - 1, f = tf+1; r >= 1 && f<=6; r--, f++){
            attackMasks[square] |= (one << (r*8 + f));
        }
        for (r=tr+1, f = tf-1; r <= 6 && f>=1; r++, f--){
            attackMasks[square] |= (one << (r*8 + f));
        }
    }
    
    
    return attackMasks;
}
uint64_t* bishopMasks =  genBishopMask();

uint64_t * genRookMask() {
    static uint64_t attackMasks[64] = {0};
    uint64_t one = 1;
    for (int square = 0; square<64; square++){
        int r, f;
    int tr = square / 8;
    int tf = square % 8;
    for (r=tr+1; r <= 6 ; r++){
    attackMasks[square] |= (one << (r*8 + tf));
    }
    for (r=tr-1; r >=1; r--){
        attackMasks[square] |= (one << (r*8 + tf));
    }
    
    for ( f = tf+1;  f<=6; f++){
        attackMasks[square] |= (one << (tr*8 + f));
    }
    for (f = tf-1; f>=1; f--){
        attackMasks[square] |= (one << (tr*8 + f));
    }
    
    }
    
    return attackMasks;
}
uint64_t * rookMasks = genRookMask();

uint64_t * genQueenMask(uint64_t * rook, uint64_t * bishop){
    static uint64_t attackMasks[64] = {0};
    for (int square = 0; square < 64; square ++){
        attackMasks[square] = rook[square] | bishop[square];
    }
    return attackMasks;
}

uint64_t set_occupancy(int index, int numBits, uint64_t attackMask){
    uint64_t tempMask = attackMask;
    uint64_t occupancy = 0;
    for (int count = 0; count < numBits; count++){
        int square = bitboardHelpers::getLSB(attackMask);
        if (index & uint64_t(1) << count){
            occupancy |= (uint64_t(1) << square);
        }
    }
    return occupancy;
}

uint64_t findMagicNum(int square, int relevantBits, int piece){

    int occupancyIndex = 1 << relevantBits;
    uint64_t occupancies[4096];
    uint64_t attacks[4096];
    uint64_t usedAttacks[4096];
    uint64_t attackMask;
    if (piece == 1){ // bishop
        attackMask = bishopMasks[square];
        
    } else if (piece == 3) { //rook
        attackMask = rookMasks[square];
    } else {
        exit(0);
    }

    
    
    uint64_t one = 1;
    
    for (int index = 0; index < occupancyIndex; index ++){
        
        occupancies[index] = set_occupancy(index, relevantBits, attackMask);
        if (piece == 1){ // bishop
            attacks[index] = genBishopAttacks(square, occupancies[index]);
        } else if (piece == 3) { //rook
            attacks[index] = genRookAttacks(square, occupancies[index]);
        }
        
    }
        
        for (int randomCount = 0; randomCount < 100000000000; randomCount++){
            uint64_t magicNum = getMagicNumCandidate();
            
            if (getBitCount((attackMask * magicNum) & 0xFF00000000000000) < 6) continue;
            memset(usedAttacks, uint64_t(0), sizeof(usedAttacks));
            int index, fail;
            for (index = 0, fail=0; !fail && index < occupancyIndex; index ++){
                int magicIndex = int((occupancies[index] * magicNum) >> (64-relevantBits));
                if (usedAttacks[magicIndex] == uint64_t(0)){
                        usedAttacks[magicIndex] = attacks[index];
                    } else if (usedAttacks[magicIndex] != attacks[index]) {
                        fail = 1;
                        break;
                    }

                }
            if (!fail){
                return magicNum;
            }
        }
        printf("Magic Number fails!");
        return uint64_t(0);
        

}

void initMagicNumbers(){

     for (int i = 0; i < 64; i++){
        bishopMagics[i] = findMagicNum(i, bishopBitCounts[i], 1);
        printf("0x%llxULL, \n", findMagicNum(i, bishopBitCounts[i], 1));
    }

    printf("\n\n");

    for (int i = 0; i < 64; i++){
        rookMagics[i] = findMagicNum(i, rookBitCounts[i], 3);
        printf("0x%llxULL, \n", findMagicNum(i, rookBitCounts[i], 3));
    }   
}


uint64_t genBishopAttacks(int square, uint64_t blockers){
    
    uint64_t attackMask = 0;
    uint64_t one = 1;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;
    for (r=tr + 1, f = tf+1; r <= 7 && f<=7; r++, f++){
        attackMask |= (one << (r*8 + f));
        if (blockers & (one << (r*8 + f))) break;
    }
    for (r=tr-1, f = tf-1; r >=0 && f>=0; r-- , f-- ){
        attackMask |= (one << (r*8 + f));
        if (blockers & (one << (r*8 + f))) break;
    }
    for (r=tr - 1, f = tf+1; r >= 0 && f<=7; r--, f++){
        attackMask |= (one << (r*8 + f));
        if (blockers & (one << (r*8 + f))) break;
    }
    for (r=tr+1, f = tf-1; r <= 7 && f>=0; r++, f--){
        attackMask |= (one << (r*8 + f));
        if (blockers & (one << (r*8 + f))) break;
    }
    return attackMask;
}

uint64_t genRookAttacks(int square, uint64_t blockers){
    uint64_t attackMask = 0;
    uint64_t tempMask = 0;
    uint64_t one = 1;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;
    bool flag;
    
    for (r=tr+1; r <= 7 ; r++){
    attackMask |= (one << (r*8 + tf));
    if (blockers & (one << (r*8 + tf))) break;
    }
    for (r=tr-1; r >=0; r--){
        attackMask |= (one << (r*8 + tf));
        if (blockers & (one << (r*8 + tf))) break;
    }
    for ( f = tf+1;  f<=7; f++){
        attackMask |= (one << (tr*8 + f));
        if (blockers & (one << (tr*8 + f))) break;
    }
    for (f = tf-1; f>=0; f--){
        attackMask |= (one << (tr*8 + f));
        if (blockers & (one << (tr*8 + f))) break;
    }
    return attackMask;
}



uint64_t get_bishop(int square, uint64_t occupancy){
    occupancy &= bishopMasks[square];
    occupancy = occupancy * bishopMagics[square];
    occupancy >>= (64-bishopBitCounts[square]);
    return bishopAttackMasks[square][occupancy];
}
uint64_t get_rook(int square, uint64_t occupancy){
    occupancy &= rookMasks[square];
    occupancy = occupancy * rookMagics[square];
    occupancy >>= (64-rookBitCounts[square]);
    return RookAttackMasks[square][occupancy];
}

