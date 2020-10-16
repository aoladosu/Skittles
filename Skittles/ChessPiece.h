#pragma once

class ChessPiece
{
    // hold data for each chess piece

private:
    int value = 0;					// value of chess piece, 0 for empty piece
    int nameVal = 0;				// chess piece's name as a value
    int color = -1;					// colour of piece 0=WHITE,1=BLACK
    bool moved = 0;					// true if the piece has been moved before. ex: castling, pawn's first move

public:
    ChessPiece();
    ChessPiece(int val, int nameval, int piece_color);
    ~ChessPiece();
    int getValue();
    int getNameValue();
    int getColor();
    bool getMoved();
    void setMoved(bool val);
};


