#include "ChessPiece.h"

ChessPiece::ChessPiece(){}

ChessPiece::~ChessPiece() {}

ChessPiece::ChessPiece(short int nameval, short int piece_color)
{
    color = piece_color;
    nameVal = nameval;
}

bool ChessPiece::getMoved()
{
    return moved;
}

void ChessPiece::setMoved(bool val)
{
    moved = val;
}

short int ChessPiece::getNameValue()
{
    return nameVal;
}

short int ChessPiece::getColor()
{
    return color;
}
