#include "MoveList.h"

MoveList::MoveList(){
    cirQueue = new Move[maxSize];
}

void MoveList::createMove(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promoTo, ChessPiece capturedPiece, bool mate, short int moved, bool check){
    // add move to the list

    Move move = Move(startPos, endPos, passantArray, passantColor, special, capture, promoTo, capturedPiece, mate, moved, check);

    if (isEmpty){
        start = 0;
        isEmpty = false;
    }

    current = (current + 1) % maxSize;
    cirQueue[current] = move;
    end = current;

    // check if queue is full and need to increment start
    if (((end + 1) % maxSize) == start){
        start = (start + 1) % maxSize;
    }
}

Move* MoveList::getPrevious(){
    // get previous move
    // previous move is technically what current is pointing to

    Move *move = nullptr;

    if ((((current + 1) % maxSize) != start) && (!isEmpty)){
        move = &cirQueue[current];
        current = (current - 1) % maxSize;
    }

    return move;
}

Move* MoveList::getNext(){
    // get next move
    // next move is the one after current

    if ((current == end) || (isEmpty)){
        return nullptr;
    }

    current = ((current + 1) % maxSize);
    Move *move = &cirQueue[current];
    return move;

}

bool MoveList::isStart(){
    // check if current pointer is at start of list
    return ((((current + 1) % maxSize) == start) || (isEmpty));
}

bool MoveList::isEnd(){
    // check if current pointer is at end of list
    return (current == end);
}

void MoveList::clear(){
    // clear the list for a new game
    current = -1;
    start = -1;
    end = -1;
    isEmpty = true;
}

MoveList::~MoveList(){
    delete[] cirQueue;
}
