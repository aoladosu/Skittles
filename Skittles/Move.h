#pragma once
#include "ChessPiece.h"

class Move
{
public:
    Move();
    Move(short int startPos, short int endPos, short int passantArray[2], short int passantColor, short int special, bool capture, short int promo, ChessPiece caturedPiece, bool mate, short int moved, bool check, bool stalemate, bool insuff);
    short int start;                      // where the piece started this move
    short int end;                        // where the piece ended this move
    short int enPassant[2] = {};          // enpassant information
    short int enPassantColor;             // enpassant information
    short int specialMove;                // special move that was active after this move completed
    bool captured;                  // whether the move captured anotherpiece or not
    short int promoTo;              // piece type piece was promoted to
    ChessPiece piece;               // piece that was captured
    bool mate;                      // whether this move caused mate
    bool check;                     // whether this move caused check
    bool stalemate;                  // whether this move caused stalemate
    bool moved;                     // whether the piece that was moved had moved before
    bool insuff;                    // whether this move caused insufficient material


    Move* getPrevious();
    Move* getNext();
    void setPrevious(Move* ptr);
    void setNext(Move* ptr);
};
