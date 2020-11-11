#include "Move.h"

Move::Move(){}

Move::Move(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promo, ChessPiece capturedPiece, bool mate, short int moved)
    : start(startPos), end(endPos), enPassantColor(passantColor), specialMove(special), captured(capture), promoTo(promo), piece(ChessPiece(capturedPiece)), mate(mate), moved(moved)
{
    enPassant[0] = passantArray[0];
    enPassant[1] = passantArray[1];
}

Move* Move::getPrevious(){
    return previous;
}

Move* Move::getNext(){
    return next;
}

void Move::setPrevious(Move* ptr){
    previous = ptr;
}

void Move::setNext(Move* ptr){
    next = ptr;
}
