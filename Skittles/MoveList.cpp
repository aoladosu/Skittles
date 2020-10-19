#include "MoveList.h"

MoveList::MoveList(){}

void MoveList::createMove(int startPos, int endPos, int passantArray[2], int passantColor, int special, bool capture, int promoTo, ChessPiece capturedPiece, bool mate){
    // add move to the list

    Move *move = new Move(startPos, endPos, passantArray, passantColor, special, capture, promoTo, capturedPiece, mate);

    if (start == nullptr){
        // there are no moves in the list
        start = move;
        current = start;
        end = start;
    }
    else{
        // already moves in list
        if (current!=end){
            remove(current->getNext());
        }
        current->setNext(move);
        move->setPrevious(current);
        current = current->getNext();
        end = current;
    }

    numMoves++;
    return;
}

Move* MoveList::getPrevious(){
    // get previous move

    if (current == start){
        // nothing in list or at start of list
        return nullptr;
    }
    else{
        current = current->getPrevious();
        numMoves--;
        return current;
    }
}

Move* MoveList::getNext(){
    // get next move

    if (current == end){
        // nothing in list or at end of list
        return nullptr;
    }
    else{
        current = current->getNext();
        numMoves++;
        return current;
    }

}

Move* MoveList::getCurrent(){
    // get current move

    return current;
}

int MoveList::getNumMoves(){
    return numMoves;
}

bool MoveList::isStart(){
    // check if current pointer is at start of list
    return (start == end);
}

bool MoveList::isEnd(){
    // check if current pointer is at end of list
    return (current == end);
}

bool MoveList::isEmpty(){
    // check if list is currently empty
    return (start == nullptr);
}

void MoveList::clear(){
    // clear the list for a new game
    remove(start);
    current = nullptr;
    start = nullptr;
    end = nullptr;
    numMoves = 0;
}

void MoveList::remove(Move *curr){
    // delete moves from [curr, end] inclusive

    if (curr == nullptr) return;

    while (curr != end){
        curr = curr->getNext();
        delete curr->getPrevious();
    }

    delete curr;

 }

MoveList::~MoveList(){
    remove(start);
}
