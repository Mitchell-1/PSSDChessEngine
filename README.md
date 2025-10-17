The program can be compiled and run through the included makefile.


When ran, the engine accepts a list of commands, some with several options. A full list and explanation has been included below.

List of commands:
quit: Exits the process.
position
    -startpos: Sets the game state to the starting position of a chess game.
    -fen: accepts a FEN string and sets the game state to the position described by the string
go
    -depth: The depth at which the engine will search when looking for the best move.
    -perft: Will run a move generation test at the given depth and return the number of nodes found.
move: Accepts a move in the format of "startingSquare""endingSquare" and updates the game/makes the move if it is valid. . e.g. e2e4.
perftSuite: Runs a series of perft commands that test for several pre-chosen positions at several depths.
test: Returns the evaluation of the current game state along with other related information. 

