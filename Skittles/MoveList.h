#pragma once
#include "Move.h"

class MoveList
{
private:

    // manage linked list
    Move *start = nullptr;
    Move *current = nullptr;
    Move *end = nullptr;
    int numMoves = 0;

public:

    MoveList();
    ~MoveList();
    void createMove(int startPos, int endPos, int passantArray[2], int passantColor, int special, bool capture, int promoTo, ChessPiece capturedPiece, bool mate);
    Move* getPrevious();
    Move* getNext();
    Move* getCurrent();
    int getNumMoves();
    bool isStart();
    bool isEnd();
    bool isEmpty();
    void clear();
    void remove(Move *curr);
};

