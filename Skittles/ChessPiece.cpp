#include "ChessPiece.h"

ChessPiece::ChessPiece(){}

ChessPiece::~ChessPiece() {}

ChessPiece::ChessPiece(int val, int nameval, int piece_color)
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

int ChessPiece::getValue()
{
    return value;
}

int ChessPiece::getNameValue()
{
    return nameVal;
}

int ChessPiece::getColor()
{
    return color;
}
