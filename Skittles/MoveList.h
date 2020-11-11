#pragma once
#include "Move.h"

class MoveList
{
private:

    // manage linked list
    Move *start = nullptr;
    Move *current = nullptr;
    Move *end = nullptr;
    short int numMoves = 0;

public:

    MoveList();
    ~MoveList();
    void createMove(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promoTo, ChessPiece capturedPiece, bool mate, short int moved);
    Move* getPrevious();
    Move* getNext();
    Move* getCurrent();
    short int getNumMoves();
    bool isStart();
    bool isEnd();
    bool isEmpty();
    void clear();
    void remove(Move *curr);
};

