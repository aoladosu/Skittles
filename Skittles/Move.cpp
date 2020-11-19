#include "Move.h"

Move::Move(){}

Move::Move(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promo, ChessPiece capturedPiece, bool mate, short int moved, bool check, bool stalemate, bool insuff)
    : start(startPos), end(endPos), enPassantColor(passantColor), specialMove(special), captured(capture), promoTo(promo), piece(ChessPiece(capturedPiece)), mate(mate), check(check), stalemate(stalemate), moved(moved), insuff(insuff)
{
    enPassant[0] = passantArray[0];
    enPassant[1] = passantArray[1];
}
