#include "ChessPiece.h"

ChessPiece::ChessPiece(){}

ChessPiece::~ChessPiece() {}

ChessPiece::ChessPiece(short int val, short int nameval, short int piece_color)
{
    value = val;
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

short int ChessPiece::getValue()
{
    return value;
}

short int ChessPiece::getNameValue()
{
    return nameVal;
}

short int ChessPiece::getColor()
{
    return color;
}
