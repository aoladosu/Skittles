#include "MoveList.h"

MoveList::MoveList(){}

void MoveList::createMove(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promoTo, ChessPiece capturedPiece, bool mate, short int moved){
    // add move to the list

    Move *move = new Move(startPos, endPos, passantArray, passantColor, special, capture, promoTo, capturedPiece, mate, moved);

    if (start == nullptr){
        // there are no moves in the list
        start = move;
        current = start;
        end = start;
    }
    else{
        // already moves in list
        if (current!=end){
            if (current != nullptr){
                remove(current->getNext());
            }
            else{
                // move list isn't empty, but game has been rewound to start
                remove(start);
                start = move;
                current = start;
                end = start;
                return;
            }
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
    // previous move is technically what current is pointing to

    Move *move = current;

    if (current != nullptr){
        current = current->getPrevious();
        numMoves--;
    }

    return move;
}

Move* MoveList::getNext(){
    // get next move
    // next move is the one after current

    if (current == end){
        // nothing in list or at end of list
        return nullptr;
    }
    else{
        if (current != nullptr){
            current = current->getNext();
        }
        else{
            // current is pointing to before the first move
            current = start;
        }
        numMoves++;
        return current;
    }

}

Move* MoveList::getCurrent(){
    // get 'current' move

    return current;
}

short int MoveList::getNumMoves(){
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
