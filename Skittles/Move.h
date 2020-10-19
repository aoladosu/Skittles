#pragma once
#include "ChessPiece.h"

class Move
{
private:
    // create linked list
    Move *previous = nullptr;
    Move *next = nullptr;

public:
    Move(int startPos, int endPos, int passantArray[2], int passantColor, int special, bool capture, int promo, ChessPiece caturedPiece, bool mate);
    int start;                      // where the piece started this move
    int end;                        // where the piece ended this move
    int enPassant[2] = {};          // enpassant information
    int enPassantColor;             // enpassant information
    int specialMove;                // special move that was active after this move completed
    bool captured;                  // whether the move captured anotherpiece or not
    int promoTo;                    // piece type piece was promoted to
    ChessPiece piece;               // piece that was captured
    bool mate;


    Move* getPrevious();
    Move* getNext();
    void setPrevious(Move* ptr);
    void setNext(Move* ptr);
};
