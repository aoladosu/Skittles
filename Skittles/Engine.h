#pragma once
#include "Board.h"

class Engine
{
    // interface to the board class
private:
    Board board;

public:

    Engine();
    void init();                                                        // initialize a new game
    bool validate(int start, int end);                                  // validate a move
    int getSpecialMove();                                               // check if a special move happened
    bool promote(int pieceNameVal);                                     // promote a piece
    bool gameOver();                                                    // check if game is over
    int getWinner();                                                    // check who won
    void goBack(int &start, int &end, int &special, int &promoPiece, int &capturedPiece, int &color);           // go back one move
    void goForward(int &start, int &end, int &special, int &promoPiece, int &capturedPiece, int &color);        // go forward one move
    void moveStats(int &pieceMoved, int &color, bool &capture, bool &check, bool &checkmate);                   // get statistics about last move
};
