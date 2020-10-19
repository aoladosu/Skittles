#include "Move.h"

Move::Move(int startPos, int endPos, int passantArray[2], int passantColor, int special, bool capture, int promo, ChessPiece capturedPiece, bool mate)
    : start(startPos), end(endPos), enPassantColor(passantColor), specialMove(special), captured(capture), promoTo(promo), piece(ChessPiece(capturedPiece)), mate(mate)
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
