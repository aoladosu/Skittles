#pragma once

class ChessPiece
{
    // hold data for each chess piece

private:
    short int value = 0;					// value of chess piece, 0 for empty piece
    short int nameVal = 0;                  // chess piece's name as a value
    short int color = -1;					// colour of piece 0=WHITE,1=BLACK
    bool moved = 0;                         // true if the piece has been moved before. ex: castling, pawn's first move

public:
    ChessPiece();
    ChessPiece(short int val, short int nameval, short int piece_color);
    ~ChessPiece();
    short int getValue();
    short int getNameValue();
    short int getColor();
    bool getMoved();
    void setMoved(bool val);
};


