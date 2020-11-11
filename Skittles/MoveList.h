#pragma once
#include "Move.h"

// move list is implemented as a circular queue

class MoveList
{
private:

    // variables
    Move *cirQueue = nullptr;
    short int maxSize = 20;
    short int current = 0;
    short int start = 0;
    short int end = 0;
    bool isEmpty = true;

public:

    MoveList();
    ~MoveList();
    void createMove(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promoTo, ChessPiece capturedPiece, bool mate, short int moved, bool check);
    Move* getPrevious();
    Move* getNext();
    bool isStart();
    bool isEnd();
    void clear();
    void setEnd();
    void addPromo(short int promo);
};

