#include "game.h"


// Converts a standard notation FEN string into bitboards.
// After reading pieces it reads in all necessary variables 
// for the game state to continue from the position.
void Game::fenToBitBoards(std::string fenPosition) {
    uint64_t square = 56;
    auto next = fenPosition[0];
    while (next != ' '){
        if (isdigit(next)){
            next = next - '0';
            square = square + int(next);
        } else {
            // If the character is not a digit, it must be a piece.
            // Determine which piece and set the corresponding bitboard.
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

    // Read in turn, castling rights and possible en passant square.
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
    // Generate initial attacks for both sides.
    // Turn is flipped as genAttacks only generates attacks for the side not to move.
    this->genAttacks();
    this->turn = this->turn ? 0 : 1;
    this->genAttacks();
    this->turn = this->turn ? 0 : 1;
}

// Compiles any changes to the individual boards into the total board.
void Game::updateMainBoards(){

    // Update the aggregate bitboards for both sides and the main board.
    whiteBoard = ((whiteBoards[0] | whiteBoards[1] | whiteBoards[2] | 
                   whiteBoards[3] | whiteBoards[4] | whiteBoards[5]));
    blackBoard = ((blackBoards[0] | blackBoards[1] | blackBoards[2] | 
                   blackBoards[3] | blackBoards[4] | blackBoards[5]));
    mainBoard = ((whiteBoard | blackBoard));
}

// Returns the piece-index value of the occupied square as specified by "square".
// Checks based on the colour given (0 = white, 1 = black).
int Game::getPieceBySquare(Bitboard square, int colour) const{
    // Check each piece bitboard of the given colour to see which contains the square.
    // If none are found return -1.
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
    return -1;
}

using namespace bitboardHelpers;

// Generates all of the attack bitboards for all pieces 
// of the side to move.
void Game::genAttacks(){
    this->updateMainBoards();
    Bitboard friendlyBoard, enemyBoard, attackBoard;
    Bitboard * friendlyBoards, *enemyBoards;
    int lsb;

    // Load all boards depending on side to move.
    // Dereference pointers to reduce code duplication.
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

    // Generate Pawn attacks
    // Split into not A and not H file to avoid wrap around attacks.
    Bitboard pawnsNotAFile = friendlyBoards[0] & (~myEngine::FileABB);
    Bitboard pawnsNotHFile = friendlyBoards[0] & (~myEngine::FileHBB);
    
    if (this->turn) {
        attackBoard |= pawnsNotAFile >> 9;
        attackBoard |= pawnsNotHFile >> 7;
        blackAttacks[0] =(attackBoard) ;
    }   else {
        attackBoard |= pawnsNotHFile<< 9;
        attackBoard |= pawnsNotAFile << 7;
        whiteAttacks[0] =(attackBoard);
    }     

    // Generate Knight attacks using precomputed bit mask 
    // of attacks for each square.
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

    // Generate Bishop attacks using magic bitboards.
    // Magic bitboards are accessed through the get_bishop function.
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

    // Generate Rook attacks using magic bitboards.
    // Magic bitboards are accessed through the get_rook function.
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

    // Generate Queen attacks by combining rook and bishop attacks.
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

    // Generate King attacks using precomputed bit mask
    // of attacks for each square.
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
    
    // Combine all attacks into one bitboard for easy checking and check determination.
    if (this->turn) {
        blackAttack = (blackAttacks[0] | blackAttacks[1] | blackAttacks[2] | blackAttacks[3] | blackAttacks[4] | blackAttacks[5]);
    } else {
        whiteAttack = (whiteAttacks[0] | whiteAttacks[1] | whiteAttacks[2] | whiteAttacks[3] | whiteAttacks[4] | whiteAttacks[5]);
    }

}

// Updates the game state and all boards by the given move.
bool Game::makeMove(Move& move){
    this->turn = this->turn ? 0 : 1;
    int from, to;
    from = move.fromSquare();
    to = move.toSquare();
    
    if (move.colour()) {
        // Update the black board by the move 
        // and white board by any capture.
        updateBoard(blackBoards[move.type()], from, to);
        int capture = move.capture();
        // If the move captures a piece, update the capture history and boards.
        // 7 represents no capture.
        // 0-6 represent piece indices.
        if (capture != 7) {
            captureHistory.push_back(movesWithoutCapture);
            movesWithoutCapture = 0;
            if (capture == 6) {
                //en passant capture
                updateBoard(whiteBoards[6], to);
                updateBoard(whiteBoards[0], to+8);
            } else {
                //standard capture
                popBit(whiteBoards[capture], to);
            }
            
        } else {
            movesWithoutCapture++;
        }

        // Check if the move broke any castling rights
        if (canWhiteKingCastle && capture == 3 && to == 7) {
            canWhiteKingCastle = false;
            move.orData((1<<2), 24); //updating the move to hold memory that it broke castle rights
        
        }
        if (canWhiteQueenCastle && capture == 3 && to == 0) {
            canWhiteQueenCastle = false;
            move.orData((1<<3), 24); //updating the move to hold memory that it broke castle rights
        }
        int type = move.type();
        if (canBlackKingCastle){
            if ((type == 5) | ((type == 3) && (from == 63))){
                canBlackKingCastle = false;
                move.orData((1), 24); //updating the move to hold memory that it broke castle rights
            }
                
        }
        if (canBlackQueenCastle) {
            if (type == 5 | ((type) == 3 && (from == 56))){
                canBlackQueenCastle = false;
                move.orData((1<<1), 24); //updating the move to hold memory that it broke castle rights
            }
        }

        // If move is double pawn push set the en-passant
        // bit on the black board to signal possible en-passant.
        if (move.passantable()){
            blackBoards[6] = (oneBB << (to+8));
        }

        // If the move is any type of special move (castle or promotion)
        // check the flag and update the boards accordingly.
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

        // Update the en-passant boards for lower depth move generation.
        // Save the the en-passant board to history for un-making moves.
        if (whiteBoards[6] != 0){
            WhitePHistory.push_back(whiteBoards[6]);
            whiteBoards[6] = (0);
        } else if (WhitePHistory.size() != 0) {
            WhitePHistory.push_back(0);
        }
        this->genAttacks();
    } else {
        // Update the White board by the move 
        // and white board by any capture.
        updateBoard(whiteBoards[move.type()], from, to);
        int capture = move.capture();
        // If the move captures a piece, update the capture history and boards.
        // 7 represents no capture.
        // 0-6 represent piece indices.
        if (capture != 7) {
            captureHistory.push_back(movesWithoutCapture);
            movesWithoutCapture = 0;
            if (capture == 6) {
                //en passant capture
                updateBoard(blackBoards[6], to);
                updateBoard(blackBoards[0], to-8);
            } else {
                popBit(blackBoards[capture], to);
            }
            
        } else {
            movesWithoutCapture++;
        }
        // Check if the move broke any castling rights
        if (canBlackKingCastle && capture == 3 && to == 63) {
            canBlackKingCastle = false;
            move.orData((1), 24); //updating the move to hold memory that it broke castle rights
        }
        if (canBlackQueenCastle && capture == 3 && to == 56) {
            canBlackQueenCastle = false;
            move.orData((1<<1), 24); //updating the move to hold memory that it broke castle rights
        }
        int type = move.type();
        if (canWhiteKingCastle){
            if (type == 5 | ((type) == 3 && (from == 7))){
                canWhiteKingCastle = false;
                move.orData((1<<2), 24); //updating the move to hold memory that it broke castle rights
            }
        }
        if (canWhiteQueenCastle) {
            if (type == 5 | ((type) == 3 && (from == 0))){
                canWhiteQueenCastle = false;
                move.orData((1<<3), 24); //updating the move to hold memory that it broke castle rights
            }    
        }

        // If move is double pawn push set the en-passant
        // bit on the white board to signal possible en-passant.
        if (move.passantable()){
            whiteBoards[6] = (oneBB << (to-8));
        }

        // If the move is any type of special move (castle or promotion)
        // check the flag and update the boards accordingly.
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

        // Update the en-passant boards for lower depth move generation.
        // Save the the en-passant board to history for un-making moves.
        if (blackBoards[6] != 0){
            BlackPHistory.push_back(blackBoards[6]);
            blackBoards[6] = (0);
        } else if (BlackPHistory.size() != 0) {
            BlackPHistory.push_back(0);
        }

        this->genAttacks();
    }
    this->updateMainBoards();
    // Generates the remaining attacks for the side not to move
    // then flips the turn back to the side to move.
    this->turn = this->turn ? 0 : 1;
    this->genAttacks();
    this->turn = this->turn ? 0 : 1;
    return false;
}


// Undoes the given move and restores the game 
// state to before the move was made. 
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
            movesWithoutCapture = captureHistory.back();
            captureHistory.pop_back();
        } else {
            movesWithoutCapture--;
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
            movesWithoutCapture = captureHistory.back();
            captureHistory.pop_back();
        } else {
            movesWithoutCapture--;
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
        int LSB = getLSB(temp);
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

// Checks if the king of the given colour is in check.
// Casts rays out from the king to see if any enemy pieces attack it.
// Returns true if in check, false otherwise.
bool Game::inCheck(bool colour) {
    Bitboard kingBB = colour ? blackBoards[5] : whiteBoards[5];
    Bitboard attackBB;
    if (colour) {
        int kingSquare = getLSB(kingBB);
        //404 indicates no king found, should not happen in a valid game.
        // Can happen during move searching if line includes king capture
        // but as position is invalid return true to avoid searching further.
        if (kingSquare == 404) {
            return true;
        }
        attackBB = get_bishop(kingSquare, this->mainBoard);
        if ((whiteBoards[2] | whiteBoards[4]) & attackBB) {
            return true;
        }
        attackBB = get_rook(kingSquare, this->mainBoard);
        if ((whiteBoards[3] | whiteBoards[4]) & attackBB) {
            return true;
        }
        attackBB = knightAttackMask[kingSquare];
        if ((whiteBoards[1]) & attackBB) {
            return true;
        }
        attackBB = 0;
        if (kingSquare%8 != 0) {
            attackBB |= oneBB << (kingSquare - 9);
        } 
        if (kingSquare%8 != 7) {
            attackBB |= oneBB << (kingSquare - 7);
        }
        if ((whiteBoards[0]) & attackBB) {
            return true;
        }
        return false;
    } else {
        int kingSquare = getLSB(kingBB);
        if (kingSquare == 404) {
            return true;
        }
        attackBB = get_bishop(kingSquare, this->mainBoard);
        if ((blackBoards[2] | blackBoards[4]) & attackBB){
            return true;
        }
        attackBB = get_rook(kingSquare, this->mainBoard);
        if ((blackBoards[3] | blackBoards[4]) & attackBB){
            return true;
        }
        attackBB = knightAttackMask[kingSquare];
        if ((blackBoards[1]) & attackBB) {
            return true;
        } 
        attackBB = 0;
        if (kingSquare%8 != 0) {
            attackBB |= oneBB << (kingSquare + 7);
        } 
        if (kingSquare%8 != 7) {
            attackBB |= oneBB << (kingSquare + 9);
        }
        if ((blackBoards[0]) & attackBB) {
            return true;
        }
        return false;
    }
}

// Tests if a move invalidates the position by putting own king in check.
bool Game::isMoveOk(Move& move){
    this->turn = this->turn ? 0 : 1;
    int from = move.fromSquare();
    int to = move.toSquare();
    bool flag = false;
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

        // If position is illegal undo changes and return true.
        this->updateMainBoards();
        if (this->inCheck(1)) {
            flag = true;
        }
        updateBoard(blackBoards[move.type()], to, from);
        if (capture != 7) {
            if (capture == 6) {
                updateBoard(whiteBoards[6], to);
                updateBoard(whiteBoards[0], to+8);
            } else {
                setBit(whiteBoards[capture], to);
            }
        }
        //this->genAttacks();
        this->updateMainBoards();
        this->turn = this->turn ? 0 : 1;
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

        // If position is illegal undo changes and return true.
        //this->genAttacks();
        this->updateMainBoards();
        if (this->inCheck(0)) {
            flag = true;
        }
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
            //this->genAttacks();
            this->turn = this->turn ? 0 : 1;
    }
    return flag;
}


// Receives a move in UCI format, converts it to a Move object
// and makes the move if it is legal.
void Game::recieveMove(std::string moveStr){
    // Loads all necessary variables to create a Move object.
    std::string strInitsquare = moveStr.substr(0, 2);
    std::string strTargetSquare = moveStr.substr(2, 4);
    Square initSquare = squareToInt(strInitsquare);
    Square targetSquare = squareToInt(strTargetSquare);
    uint8_t type = 0;
    uint8_t colour = 0;
    uint8_t capture = 7;
    bool check = 0;
    uint8_t promotion = 0;
    bool passantable = 0;
    int castle = 0;

    // If the initial square contains a white piece.
    if (whiteBoard & (uint64_t(1) << initSquare)){
        colour = 0;

        // Test is the move is a promotion or double pawn push.
        type = getPieceBySquare(uint64_t(1) << initSquare, 0);
        if (type == 0){
            if (targetSquare/8 == 7) {
                promotion = uint8_t(moveStr[4] - '0');
            }
            if (targetSquare - initSquare == 16) {
                passantable = true;
            }
        }
        // Test if the move is a castle and which type.
        if (type == 5) {
            int diff = targetSquare - initSquare;
            if (diff == -2) {
                castle = 2;
            } else if (diff == 2) {
                castle = 1;
            }
        }
        // Check if the target square contains an enemy piece for capture.
        if ((blackBoard & (uint64_t(1) << targetSquare)) || (blackBoards[6] & (uint64_t(1) << targetSquare)) ){
            capture = getPieceBySquare((uint64_t(1) << targetSquare), 1);
        }
    } else if (blackBoard & (uint64_t(1) << initSquare)){
        colour = 1;
        // Test is the move is a promotion or double pawn push.
        type = getPieceBySquare(uint64_t(1) << initSquare, 1);
        if (type == 0){
            if (targetSquare/8 == 0) {
                promotion = uint8_t(moveStr[4] - '0');
            }
            if (initSquare - targetSquare == 16) {
                passantable = true;
            }
        }
        // Test if the move is a castle and which type.
        if (type == 5) {
            int diff = targetSquare - initSquare;
            if (diff == -2) {
                castle = 2;
            } else if (diff == 2) {
                castle = 1;
            }
        }
        // Check if the target square contains an enemy piece for capture.
        if ((whiteBoard & (uint64_t(1) << targetSquare)) || (whiteBoards[6] & (uint64_t(1) << targetSquare))){
            capture = getPieceBySquare((uint64_t(1) << targetSquare), 0);
        }
    } else {
        printf("Input a valid move\n");
        return;
    }
    // Set the flag for the move, utilised in makeMove to efficiently determine special moves.
    uint8_t flag = 0;
    if (promotion != 0) {
        flag = 1;
    } else if (castle != 0) {
        flag = castle + 1;
    }
    // Create the Move object and test if it is legal.
    Move newMove = Move(initSquare, targetSquare, promotion, flag, type, capture, colour, passantable);
    if (isMoveOk(newMove)){
        printf("Input a valid move\n");
        return;
    }
    this->makeMove(newMove);
}


// Initializes the Zobrist hash for the current position.
void Game::initHash() {
    for (int i = 0; i < 781; i++) {
        hashes[i] = getRandomU64();
    }
    
    for (int i = 0; i < 6; i++) {
        Bitboard whitePieceBB = whiteBoards[i];
        Bitboard blackPieceBB = blackBoards[i];
        while (whitePieceBB) {
            int lsb = bitboardHelpers::getLSB(whitePieceBB);
            hash = hash ^ hashes[i * 64 + lsb];
        }
        while (blackPieceBB) {
            int lsb = bitboardHelpers::getLSB(blackPieceBB);
            hash = hash ^ hashes[blackOffset + i * 64 + lsb];
        }
        hash = hash & hashes[769] * turn;
        hash = hash & hashes[770] * canWhiteKingCastle;
        hash = hash & hashes[771] * canWhiteQueenCastle;
        hash = hash & hashes[772] * canBlackKingCastle;
        hash = hash & hashes[773] * canBlackQueenCastle;
        if (whiteBoards[6] != 0) {
            Bitboard lsbBB = whiteBoards[6];
            int lsb = bitboardHelpers::getLSB(lsbBB);
            hash = hash ^ hashes[774 + lsb%8];
        }
        if (blackBoards[6] != 0) {
            Bitboard lsbBB = blackBoards[6];
            int lsb = bitboardHelpers::getLSB(lsbBB);
            hash = hash ^ hashes[774 + lsb%8];
        }
    }
};

// Updates the Zobrist hash by the given move.
void Game::updateHash(Move& move) {
    if (move.colour()) {
        hash = hash ^ hashes[blackOffset + move.type() * 64 + move.fromSquare()];
        hash = hash ^ hashes[blackOffset + move.type() * 64 + move.toSquare()];
    } else {
        hash = hash ^ hashes[move.type() * 64 + move.fromSquare()];
        hash = hash ^ hashes[move.type() * 64 + move.toSquare()];
    }
    int capture = move.capture();
    if (capture != 7) {
        if (capture == 6) {
            if (move.colour()) {
                hash = hash ^ hashes[capture * 64 + move.toSquare()+8];
            } else {
                hash = hash ^ hashes[blackOffset + capture * 64 + move.toSquare()-8];
            }
        } else {
            if (move.colour()) {
                hash = hash ^ hashes[capture * 64 + move.toSquare()];
            } else {
                hash = hash ^ hashes[blackOffset + capture * 64 + move.toSquare()];
            }
        }
    }
    hash = hash ^ hashes[769];
};

