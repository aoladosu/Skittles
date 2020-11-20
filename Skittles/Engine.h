#pragma once
#include "Board.h"

class Engine
{
    // interface to the board class to expose only necessary functions
private:
    Board board;

public:

    Engine();
    void init();                                                        // initialize a new game
    bool validate(short int start, short int end);                      // validate a move
    short int getSpecialMove();                                         // check if a special move happened
    bool promote(short int pieceNameVal);                               // promote a piece
    bool gameOver();                                                    // check if game is over
    short int getWinner();                                              // check who won
    short int getWinReason();                                           // get what caused the win
    bool isStart();                                                     // check if there is a previous move
    bool isEnd();                                                       // check if there is a next move
    void checkPositions(short int pos[]);                          // get positions of check
    bool genMovesForPiece(short int pos, short int moves[], short int cMoves[], short int aMoves[]);                                                // get moves for piece
    void goBack(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color);           // go back one move
    void goForward(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color);        // go forward one move
    void moveStats(short int &pieceMoved, short int &color, bool &capture, bool &check, bool &checkmate);                                           // get statistics about last move
};
