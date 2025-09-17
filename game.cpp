#include "game.h"

void Game::fenToBitBoards(std::string fenPosition) {
    uint64_t square = 56;
    auto next = fenPosition[0];
    while (next != ' '){
        if (isdigit(next)){
            next = next - '0';
            square = square + int(next);
        } else {
            switch (myEngine::hashing(next))
            {
            case 0:
                whiteBoards[0]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 1:
                whiteBoards[1]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 2:
                whiteBoards[2]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 3:
                whiteBoards[3]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 4:
                whiteBoards[4]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 5:
                whiteBoards[5]|= (myEngine::intToBitBoard(square));
                square ++;
                break;
            case 6:
                blackBoards[0]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 7:
                blackBoards[1]|= (myEngine::intToBitBoard(square));
                square ++;
                break;
            case 8:
                blackBoards[2]|= (myEngine::intToBitBoard(square));
                square ++;
                break;
            case 9:
                blackBoards[3] |= (myEngine::intToBitBoard(square));
                square ++;
                break;
            case 10:
                blackBoards[4]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 11:
                blackBoards[5]|= (myEngine::intToBitBoard(square)); 
                square ++;
                break;
            case 12:
                square = square - 16;
                break;
            default:
                break;
            }
        }
        fenPosition.erase(0, 1);
        next = fenPosition[0];
    }
    fenPosition.erase(0, 1);
    next = fenPosition[0];
    if (next == 'w') {
        this->turn = 0;
    } else {
        this->turn = 1;
    }
    fenPosition.erase(0, 2);
    next = fenPosition[0];
    while (next != ' ') {
        if (next == 'K'){
            canWhiteKingCastle = true;
        } else if (next == 'Q'){
            canWhiteQueenCastle = true;
        } else if (next == 'k'){
            canBlackKingCastle = true;
        } else if (next == 'q')  {
            canBlackQueenCastle = true;
        }
        fenPosition.erase(0, 1);
        next = fenPosition[0];
    }
    fenPosition.erase(0, 1);
    next = fenPosition[0];
    if (next == '-') {

    } else {
        int squareNum = myEngine::strToBitBoard(fenPosition[0], fenPosition[1]);
        if (squareNum/8 == 2) {
            whiteBoards[6] |= (squareNum);
        } else {
            blackBoards[6] |= (squareNum);
        }
    }
    updateMainBoards();
    this->genAttacks();
}

void Game::updateMainBoards(){

    whiteBoard = ((whiteBoards[0] | whiteBoards[1] | whiteBoards[2] | 
                   whiteBoards[3] | whiteBoards[4] | whiteBoards[5]));
    blackBoard = ((blackBoards[0] | blackBoards[1] | blackBoards[2] | 
                   blackBoards[3] | blackBoards[4] | blackBoards[5]));
    mainBoard = ((whiteBoard | blackBoard));
}

int Game::getPieceBySquare(Bitboard square, int colour) const{
    if (colour == 0){
        for (int i=0; i<7; i++){
            if ((whiteBoards[i] & square) != 0){
                return i;
            }
        }
    } else {
        for (int i=0; i<7; i++){
            if ((blackBoards[i] & square) != 0){
                return i;
            }
        }
    }
}

using namespace bitboardHelpers;
void Game::updateAttacks(Move move) {
    int type = move.type();
    int to = move.toSquare();
    int from = move.fromSquare();
    int capture = move.capture();
    int lsb;
    Bitboard lsbBB;
    int typeP;
    Bitboard attack = 0;
    Bitboard prevAttack = 0;
    Bitboard capturedAttack = 0;
    Bitboard bRay, rRay;
    if (this->debug) {
        //printBitBoard(whiteAttack);
    }
    if (move.colour()) {
        if (capture !=7) {
            switch (capture)
            {
            case 0:
                if (to%8 != 0) {
                    capturedAttack |= oneBB << (to +7);
                }
                if (to%8 != 7) {
                    capturedAttack |= oneBB << (to +9);
                }
                whiteAttacks[0] |= capturedAttack;
                whiteAttacks[0] ^= capturedAttack;
                break;
            case 1:
                capturedAttack = knightAttackMask[to];
                whiteAttacks[1] |= capturedAttack;
                whiteAttacks[1] ^= capturedAttack;
                break;   
            case 2:
                capturedAttack = get_bishop(to, this->mainBoard);
                whiteAttacks[2] |= capturedAttack;
                whiteAttacks[2] ^= capturedAttack;
                break;
            case 3:
                capturedAttack = get_rook(to, this->mainBoard);
                whiteAttacks[3] |= capturedAttack;
                whiteAttacks[3] ^= capturedAttack;
                break; 
            case 4:
                capturedAttack = get_bishop(to, this->mainBoard);
                capturedAttack |= get_rook(to, this->mainBoard);
                whiteAttacks[4] |= capturedAttack;
                whiteAttacks[4] ^= capturedAttack;
                break; 
            default:
                break;
            }
        }
        
        switch (type)
        {
        case 0:
            if (from%8 != 0) {
                prevAttack |= (oneBB << (from -9));
            }
            if (from%8 != 7) {
                prevAttack |= (oneBB << (from -7));
            }
            if (to%8 != 0) {
                attack |= (oneBB << (to -9));
            }
            if (to%8 != 7) {
                attack |= (oneBB << (to -7));
            }
            blackAttacks[0] ^= prevAttack;
            blackAttacks[0] |= attack;
            break;
        case 1: 
            attack = knightAttackMask[to];
            blackAttacks[1] ^= knightAttackMask[from];
            blackAttacks[1] |= attack;
            break;
        case 2: 
            attack = get_bishop(to, this->mainBoard);
            blackAttacks[2] ^= get_bishop(from, this->mainBoard);
            blackAttacks[2] |= attack;
            break;
        case 3: 
            attack = get_rook(to, this->mainBoard);
            blackAttacks[3] ^= get_rook(from, this->mainBoard);
            blackAttacks[3] |= attack;
            break;
        case 4: 
            attack = get_bishop(to, this->mainBoard);
            attack |= get_rook(to, this->mainBoard);
            blackAttacks[4] ^= get_bishop(from, this->mainBoard);
            blackAttacks[4] ^= get_rook(from, this->mainBoard);
            blackAttacks[4] |= attack;
            break;
        case 5: 
            attack = kingAttackMask[to];
            blackAttacks[5] = attack;
            break;
        default:
            break;
        }
    } else {
        switch (type)
        {
        case 0:
            if (from%8 != 0) {
                prevAttack |= oneBB << (from +7);
            }
            if (from%8 != 7) {
                prevAttack |= oneBB << (from +9);
            }
            if (to%8 != 0) {
                attack |= oneBB << (to +7);
            }
            if (to%8 != 7) {
                attack |= oneBB << (to +9);
            }
            whiteAttacks[0] ^= prevAttack;
            whiteAttacks[0] |= attack;
            break;
        case 1: 
            attack = knightAttackMask[to];
            whiteAttacks[1] ^= knightAttackMask[from];
            whiteAttacks[1] |= attack;
            break;
        case 2: 
            attack = get_bishop(to, this->mainBoard);
            whiteAttacks[2] ^= get_bishop(from, this->mainBoard);
            whiteAttacks[2] |= attack;
            break;
        case 3: 
            attack = get_rook(to, this->mainBoard);
            whiteAttacks[3] ^= get_rook(from, this->mainBoard);
            whiteAttacks[3] |= attack;
            break;
        case 4: 
            attack = get_bishop(to, this->mainBoard);
            attack |= get_rook(to, this->mainBoard);
            whiteAttacks[4] ^= get_bishop(from, this->mainBoard);
            whiteAttacks[4] ^= get_rook(from, this->mainBoard);
            whiteAttacks[4] |= attack;
            break;
        case 5: 
            attack = kingAttackMask[to];
            whiteAttacks[5] = attack;
            break;
        default:
            break;
        }
        if (capture !=7) {
            switch (capture)
            {
            case 0:
                if (to%8 != 0) {
                    capturedAttack |= oneBB << (to +7);
                }
                if (to%8 != 7) {
                    capturedAttack |= oneBB << (to +9);
                }
                blackAttacks[0] |= capturedAttack;
                blackAttacks[0] ^= capturedAttack;
                break;
            case 1:
                capturedAttack = knightAttackMask[to];
                blackAttacks[1] |= capturedAttack;
                blackAttacks[1] ^= capturedAttack;
                break;   
            case 2:
                capturedAttack = get_bishop(to, this->mainBoard);
                blackAttacks[2] |= capturedAttack;
                blackAttacks[2] ^= capturedAttack;
                break;
            case 3:
                capturedAttack = get_rook(to, this->mainBoard);
                blackAttacks[3] |= capturedAttack;
                blackAttacks[3] ^= capturedAttack;
                break; 
            case 4:
                capturedAttack = get_bishop(to, this->mainBoard);
                capturedAttack |= get_rook(to, this->mainBoard);
                blackAttacks[4] |= capturedAttack;
                blackAttacks[4] ^= capturedAttack;
                break; 
            default:
                break;
            }
        }
    }
        bRay = get_bishop(from, this->mainBoard);
        bRay &= this->mainBoard;
        while (bRay) {
            lsb = bitboardHelpers::getLSB(bRay);
            lsbBB = (oneBB << lsb);
            if(lsbBB & whiteBoards[2]){
                whiteAttacks[2] |= get_bishop(lsb, this->mainBoard);
            }
            if(lsbBB & whiteBoards[4]){
                whiteAttacks[4] |= get_bishop(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[2]){
                blackAttacks[2] |= get_bishop(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[4]){
                blackAttacks[4] |= get_bishop(lsb, this->mainBoard);
            }
        }
        bRay = get_bishop(to, this->mainBoard);
        bRay &= this->mainBoard;
        while (bRay) {
            lsb = bitboardHelpers::getLSB(bRay);
            lsbBB = (oneBB << lsb);
            if(lsbBB & whiteBoards[2]){
                popBit(whiteAttacks[2], bishopAttackMasks[to][0]);
                whiteAttacks[2] |= get_bishop(lsb, this->mainBoard);
            }
            if(lsbBB & whiteBoards[4]){
                popBit(whiteAttacks[4], bishopAttackMasks[to][0]);
                whiteAttacks[4] |= get_bishop(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[2]){
                popBit(blackAttacks[2], bishopAttackMasks[to][0]);
                blackAttacks[2] |= get_bishop(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[4]){
                popBit(blackAttacks[4], bishopAttackMasks[to][0]);
                blackAttacks[4] |= get_bishop(lsb, this->mainBoard);
            }
        }


        rRay = get_rook(from, this->mainBoard);
        rRay &= this->mainBoard;
        while (rRay) {
            lsb = bitboardHelpers::getLSB(rRay);
            lsbBB = (oneBB << lsb);
            if(lsbBB & whiteBoards[3]){
                whiteAttacks[3] |= get_rook(lsb, this->mainBoard);
            }
            if(lsbBB & whiteBoards[4]){
                whiteAttacks[4] |= get_rook(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[3]){
                blackAttacks[3] |= get_rook(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[4]){
                blackAttacks[4] |= get_rook(lsb, this->mainBoard);
            }
        }
        rRay = get_rook(to, this->mainBoard);
        rRay &= this->mainBoard;
        while (rRay) {
            lsb = bitboardHelpers::getLSB(rRay);
            lsbBB = (oneBB << lsb);
            if(lsbBB & whiteBoards[3]){
                popBit(whiteAttacks[3], RookAttackMasks[to][0]);
                whiteAttacks[3] |= get_rook(lsb, this->mainBoard);
            }
            if(lsbBB & whiteBoards[4]){
                popBit(whiteAttacks[4], RookAttackMasks[to][0]);
                whiteAttacks[4] |= get_rook(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[3]){
                popBit(blackAttacks[3], RookAttackMasks[to][0]);
                blackAttacks[3] |= get_rook(lsb, this->mainBoard);
            }
            if(lsbBB & blackBoards[4]){
                popBit(blackAttacks[4], RookAttackMasks[to][0]);
                blackAttacks[4] |= get_rook(lsb, this->mainBoard);
            }
        }
        blackAttack = (blackAttacks[0] | blackAttacks[1] | blackAttacks[2] | blackAttacks[3] | blackAttacks[4] | blackAttacks[5]);
        whiteAttack = (whiteAttacks[0] | whiteAttacks[1] | whiteAttacks[2] | whiteAttacks[3] | whiteAttacks[4] | whiteAttacks[5]);
        if (this->debug) {
            //printBitBoard(whiteAttack);
        }
    }


void Game::genAttacks(){
    this->updateMainBoards();
    Bitboard friendlyBoard, enemyBoard, attackBoard;
    Bitboard * friendlyBoards, *enemyBoards;
    int lsb;
    attackBoard = 0;
    if (this->turn) {
        friendlyBoard = blackBoard;
        friendlyBoards = blackBoards;
        enemyBoard = whiteBoard;
        enemyBoards = whiteBoards;
    } else {
        friendlyBoard = whiteBoard;
        friendlyBoards = whiteBoards;
        enemyBoard = blackBoard;
        enemyBoards = blackBoards;
    }
    Bitboard pawnsNotAFile = friendlyBoards[0] & (~myEngine::FileABB);
    Bitboard pawnsNotHFile = friendlyBoards[0] & (~myEngine::FileHBB);
    
    //myEngine::printBitBoard(pawnsNotHFile);
    //getchar();
    if (this->turn) {
        attackBoard |= pawnsNotAFile >> 9;
        attackBoard |= pawnsNotHFile >> 7;
        blackAttacks[0] =(attackBoard) ;
    }   else {
        attackBoard |= pawnsNotHFile<< 9;
        attackBoard |= pawnsNotAFile << 7;
        whiteAttacks[0] =(attackBoard);
    }     
    attackBoard = 0;


    Bitboard knightBoard = friendlyBoards[1];
    while (knightBoard) {
        lsb = bitboardHelpers::getLSB(knightBoard);
        attackBoard |= knightAttackMask[lsb];
    }
    if (this->turn) {
        blackAttacks[1] = attackBoard;
    } else {
        whiteAttacks[1] = attackBoard;
    }

    attackBoard = 0;
    Bitboard bishopBoard = friendlyBoards[2];
    while (bishopBoard != 0) {
        lsb = bitboardHelpers::getLSB(bishopBoard);
        attackBoard |= get_bishop(lsb, this->mainBoard);
    }
    if (this->turn) {
        blackAttacks[2] = attackBoard;
    } else {
        whiteAttacks[2] = attackBoard;
    }
    attackBoard = 0;

    Bitboard rookBoard = friendlyBoards[3];
    while (rookBoard != 0) {
        lsb = bitboardHelpers::getLSB(rookBoard);
        attackBoard |= get_rook(lsb, this->mainBoard);
    }
    if (this->turn) {
        blackAttacks[3] = attackBoard;
    } else {
        whiteAttacks[3] = attackBoard;
    }
    attackBoard = 0;

    Bitboard queenBoard = friendlyBoards[4];
    while (queenBoard != 0) {
        lsb = bitboardHelpers::getLSB(queenBoard);
        attackBoard |= get_rook(lsb, this->mainBoard);
        attackBoard |= get_bishop(lsb, this->mainBoard);
    }
    if (this->turn) {
        blackAttacks[4] = attackBoard;
    } else {
        whiteAttacks[4] = attackBoard;
    }
    attackBoard = 0;

    Bitboard kingBoard = friendlyBoards[5];
    lsb = bitboardHelpers::getLSB(kingBoard);
    attackBoard |= kingAttackMask[lsb];
    if (this->turn) {
        blackAttacks[5] = attackBoard;
    } else {
        whiteAttacks[5] = attackBoard;
    }
    attackBoard = 0;
    
    if (this->turn) {
        blackAttack = (blackAttacks[0] | blackAttacks[1] | blackAttacks[2] | blackAttacks[3] | blackAttacks[4] | blackAttacks[5]);
    } else {
        whiteAttack = (whiteAttacks[0] | whiteAttacks[1] | whiteAttacks[2] | whiteAttacks[3] | whiteAttacks[4] | whiteAttacks[5]);
    }

}


using namespace bitboardHelpers;
bool Game::makeMove(Move& move){
    this->turn = this->turn ? 0 : 1;
    int from, to;
    from = move.fromSquare();
    to = move.toSquare();
    
    if (move.colour()) {
        updateBoard(blackBoards[move.type()], from, to);
        int capture = move.capture();
        if (capture != 7) {
            if (capture == 6) {
                //en passant capture
                updateBoard(whiteBoards[6], to);
                updateBoard(whiteBoards[0], to+8);
            } else {
                popBit(whiteBoards[capture], to);
            }
            
        }

        //if position is not okay undo changes and return true
        this->genAttacks();
        if (blackBoards[5] & whiteAttack) {
            updateBoard(blackBoards[move.type()], to, from);
            if (capture != 7) {
                if (capture == 6) {
                    updateBoard(whiteBoards[6], to);
                    updateBoard(whiteBoards[0], to+8);
                } else {
                    setBit(whiteBoards[capture], to);
                }
            }
            this->updateMainBoards();
            this->turn = this->turn ? 0 : 1;
            return true;
        }
        
        

        //check if broke castle
        if (canWhiteKingCastle && capture == 3 && to == 7) {
            canWhiteKingCastle = false;
            move.orData((1<<2), 24); //updating the move to hold memory that it broke castle rights
        
        }
        if (canWhiteQueenCastle && capture == 3 && to == 0) {
            canWhiteQueenCastle = false;
            move.orData((1<<3), 24);
        }
        int type = move.type();
        if (canBlackKingCastle){
            if ((type == 5) | ((type == 3) && (from == 63))){
                canBlackKingCastle = false;
                move.orData((1), 24);
            }
                
        }
        if (canBlackQueenCastle) {
            if (type == 5 | ((type) == 3 && (from == 56))){
                canBlackQueenCastle = false;
                move.orData((1<<1), 24);
            }
        }

        if (move.passantable()){
            blackBoards[6] = (oneBB << (to+8));
        }

        int special = move.special();
        switch (special)
        {
        case 1:
            //promotion
            updateBoard(blackBoards[move.type()], to);
            updateBoard(blackBoards[move.promotion()+1], to);
            break;
        case 2:
            // king castle
            flipBit(blackBoards[3], blackKingRook);
            
            break;
        case 3:
            // queen castle
            flipBit(blackBoards[3], blackQueenRook);
            break;
        default:
            break;
        }

        if (whiteBoards[6] != 0){
            WhitePHistory.push_back(whiteBoards[6]);
            whiteBoards[6] = (0);
        } else if (WhitePHistory.size() != 0) {
            WhitePHistory.push_back(0);
        }
        

    } else {
        updateBoard(whiteBoards[move.type()], from, to);
        int capture = move.capture();
        if (capture != 7) {
            if (capture == 6) {
                //en passant capture
                updateBoard(blackBoards[6], to);
                updateBoard(blackBoards[0], to-8);
            } else {
                popBit(blackBoards[capture], to);
            }
            
        }

        //if position is not okay undo changes and return true
        this->genAttacks();
        if (whiteBoards[5] & blackAttack) {
            updateBoard(whiteBoards[move.type()], to, from);
            if (capture != 7) {
                if (capture == 6) {
                    updateBoard(blackBoards[6], to);
                    updateBoard(blackBoards[0], to-8);
                } else {
                    setBit(blackBoards[capture], to);
                }
            }
            
            this->updateMainBoards();
            this->genAttacks();
            this->turn = this->turn ? 0 : 1;
            return true;
        }
        
        

        //check if broke castle
        if (canBlackKingCastle && capture == 3 && to == 63) {
            canBlackKingCastle = false;
            move.orData((1), 24);
        }
        if (canBlackQueenCastle && capture == 3 && to == 56) {
            canBlackQueenCastle = false;
            move.orData((1<<1), 24);
        }
        int type = move.type();
        if (canWhiteKingCastle){
            if (type == 5 | ((type) == 3 && (from == 7))){
                canWhiteKingCastle = false;
                move.orData((1<<2), 24);
            }
        }
        if (canWhiteQueenCastle) {
            if (type == 5 | ((type) == 3 && (from == 0))){
                canWhiteQueenCastle = false;
                move.orData((1<<3), 24);
            }    
        }

        if (move.passantable()){
            whiteBoards[6] = (oneBB << (to-8));
        }

        int special = move.special();
        switch (special)
        {
        case 1:
            //promotion
            updateBoard(whiteBoards[move.type()], to);
            updateBoard(whiteBoards[move.promotion()+1], to);
            break;
        case 2:
            // king castle
            flipBit(whiteBoards[3], WhiteKingRook);
            
            break;
        case 3:
            // queen castle
            flipBit(whiteBoards[3], WhiteQueenRook);
            break;
        default:
            break;
        }

        if (blackBoards[6] != 0){
            BlackPHistory.push_back(blackBoards[6]);
            blackBoards[6] = (0);
        } else if (BlackPHistory.size() != 0) {
            BlackPHistory.push_back(0);
        }
    }
    this->updateMainBoards();
    this->turn = this->turn ? 0 : 1;
    this->genAttacks();
    this->turn = this->turn ? 0 : 1;
    //if (whiteBoards[0] & Rank6BB || whiteBoards[6] & ~Rank3BB) {
    //    printMove(move);
    //    //printBitBoard(whiteBoards[6]);
    //    getchar();
    //}
    return false;
}

void Game::unMakeMove(Move& move){
    this->turn = this->turn ? 0 : 1;
    int from, to;
    from = move.fromSquare();
    to = move.toSquare();
    if (move.colour()) {
        updateBoard(blackBoards[move.type()], to, from);
        int special = move.special();
        switch (special)
        {
        case 1:
            popBit(blackBoards[move.promotion()+1], to);
            break;
        case 2:
            flipBit(blackBoards[3], blackKingRook);
            break;
        case 3:
            flipBit(blackBoards[3], blackQueenRook);
            break;
        default:
            break;
        }

        int capture = move.capture();
        if (capture != 7) {
            if (capture == 6) {
                setBit(whiteBoards[6], to);
                setBit(whiteBoards[0], to+8);
            } else {
                setBit(whiteBoards[capture], to);
            }
        }
        if (move.passantable()){
            blackBoards[6] = 0;
        }
        if (WhitePHistory.size() != 0) {
            if (WhitePHistory.back()!= 0) {
                whiteBoards[6] = WhitePHistory.back();
            }
            WhitePHistory.pop_back();
        }
    } else {
        updateBoard(whiteBoards[move.type()], to, from);

        int special = move.special();
        switch (special)
        {
        case 1:
            popBit(whiteBoards[move.promotion()+1], to);
            break;
        case 2:
            flipBit(whiteBoards[3], WhiteKingRook);
            break;
        case 3:
            flipBit(whiteBoards[3], WhiteQueenRook);
            break;
        default:
            break;
        }

        int capture = move.capture();
        if (capture != 7) {
            if (capture == 6) {
                setBit(blackBoards[6], to);
                setBit(blackBoards[0], to-8);
            } else {
                setBit(blackBoards[capture], to);
            }
        }
        if (move.passantable()){
            whiteBoards[6] = 0;
        }
        if (BlackPHistory.size() != 0) {
            if (BlackPHistory.back()!= 0) {
                blackBoards[6] = BlackPHistory.back();
            }
            BlackPHistory.pop_back();
        }
    }

    Bitboard temp = move.brokeCastle();
    while (temp != 0){
        //printf("start - ");
        //printMove(move);
        //std::cout << std::bitset<8>(temp) << std::endl;
        int LSB = getLSB(temp);
        //printf("LSB: %d\n", LSB);
        switch (LSB)
        {
        case 0:
            this->canBlackKingCastle = true;
            break;
        case 1:
            this->canBlackQueenCastle = true;
            break;
        case 2:
            this->canWhiteKingCastle = true;
            break;
        case 3:
            this->canWhiteQueenCastle = true;
            break;
        default:
            break;
        }
    }
        
        this->updateMainBoards();
        this->genAttacks();
        
}

bool Game::isPosOk(Move& move){
    bool flag = true;
    int from = move.fromSquare();
    int to = move.toSquare();
    int capture = move.capture();
    int lsb;
    if (move.colour()) {
        updateBoard(blackBoards[move.type()], from, to);

        if (capture != 7){
            if (capture == 6) {
                updateBoard(whiteBoards[6], to);
                updateBoard(whiteBoards[0], to+8);
            } else {
                popBit(whiteBoards[capture], to);
            }
        }
        this->updateMainBoards();
        Bitboard kingBB = blackBoards[5];
        int king = getLSB(kingBB);

        Bitboard bRayBB = get_bishop(king, this->mainBoard);
        bRayBB &= this->whiteBoard;
        
        while (bRayBB) {
            lsb = getLSB(bRayBB);
            if (((oneBB << lsb) & whiteBoards[2]) || ((oneBB << lsb) & whiteBoards[4])) {
                flag = false;
            }
        }

        Bitboard rRayBB = get_rook(king, this->mainBoard);
        rRayBB &= this->whiteBoard;
        
        while (rRayBB) {
            lsb = getLSB(rRayBB);
            if (((oneBB << lsb) & whiteBoards[3]) || ((oneBB << lsb) & whiteBoards[4])) {
                flag = false;
            }
        }

        Bitboard nRayBB = knightAttackMask[king];
        nRayBB &= whiteBoard;
        while (nRayBB) {
            lsb = getLSB(nRayBB);
            if (((oneBB << lsb) & whiteBoards[1])) {
                flag = false;
            }
        }
        if (blackBoards[5] & ~myEngine::FileHBB) {
            if (blackBoards[5] >> 7 & whiteBoards[0]) {
                flag = false;
            }
        }
        if (blackBoards[5] & ~myEngine::FileABB) {
            if (blackBoards[5] >> 9 & whiteBoards[0]) {
                flag = false;
            }
        }

        if (capture != 7) {
            if (capture == 6) {
                setBit(whiteBoards[6], to);
                setBit(whiteBoards[0], to+8);
            } else {
                setBit(whiteBoards[capture], to);
            }
        }

        updateBoard(blackBoards[move.type()], to, from);
        this->updateMainBoards();
    } else {
        updateBoard(whiteBoards[move.type()], from, to);

        if (capture != 7){
            if (capture == 6) {
                updateBoard(blackBoards[6], to);
                updateBoard(blackBoards[0], to-8);
            } else {
                popBit(blackBoards[capture], to);
            }
        }
        this->updateMainBoards();
        Bitboard kingBB = whiteBoards[5];
        int king = getLSB(kingBB);

        Bitboard bRayBB = get_bishop(king, this->mainBoard);
        bRayBB &= this->blackBoard;
        
        while (bRayBB) {
            lsb = getLSB(bRayBB);
            if (((oneBB << lsb) & blackBoards[2]) || ((oneBB << lsb) & blackBoards[4])) {
                flag = false;
            }
        }

        Bitboard rRayBB = get_rook(king, this->mainBoard);
        rRayBB &= blackBoard;
        
        while (rRayBB) {
            lsb = getLSB(rRayBB);
            if (((oneBB << lsb) & blackBoards[3]) || ((oneBB << lsb) & blackBoards[4])) {
                flag = false;
            }
        }

        Bitboard nRayBB = knightAttackMask[king];
        while (nRayBB) {
            lsb = getLSB(nRayBB);
            if (((oneBB << lsb) & blackBoards[1])) {
                flag = false;
            }
        }
        if (whiteBoards[5] & ~myEngine::FileHBB) {
            if (whiteBoards[5] << 9 & blackBoards[0]) {
                flag = false;
            }
        }
        if (whiteBoards[5] & ~myEngine::FileABB) {
            if (whiteBoards[5] << 7 & blackBoards[0]) {
                flag = false;
            }
        }

        if (capture != 7) {
            if (capture == 6) {
                setBit(blackBoards[6], to);
                setBit(blackBoards[0], to-8);
            } else {
                setBit(blackBoards[capture], to);
            }
        }
        
        updateBoard(whiteBoards[move.type()], to, from);
        this->updateMainBoards();
    }
    return flag;
}
