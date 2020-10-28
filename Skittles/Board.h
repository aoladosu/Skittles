#pragma once
#include "ChessPiece.h"
#include "MoveList.h"

class Board
{
    // manage the board, by validating moves and moving pieces

private:

    // variables for piece type
    const int BOARDSIZE = 8;
    const int BLACK = 1;
    const int WHITE = 0;
    const int EMPTY = 0;
    const int PAWN = 1;
    const int ROOK = 2;
    const int KNIGHT = 3;
    const int BISHOP = 4;
    const int QUEEN = 5;
    const int KING = 6;

    // tracking enpassant variables
    int enPassantPos[2] = {-2,-2};	// all manipulations will stay out of bounds
    int enPassantColor = -1;
    int enPassantPosOld[2] = { -2,-2 };	// stores the old cycle of enpassant
    int enPassantColorOld = -1;

    // board state
    bool MATE = false;									// endgame conditions
    int toPlay = WHITE;									// what side is playing
    int bkPos[2] = {0, 4}, wkPos[2] = {7, 4};			// king positions
    int numPieces = 32;
    int numPiecesOld = 32;

    // special moves
    const int NOSPECIAL = 0;
    const int PROMOTION = 1;
    const int ENPASSANT = 2;
    const int BQCASTLE = 3;
    const int WQCASTLE = 4;
    const int BKCASTLE = 5;
    const int WKCASTLE = 6;
    int specialMove = NOSPECIAL;
    int promotionPos[2] = { -2, -2 };

    // variables for movelist
    MoveList moveList;                                  // store moves played
    bool movedStore;                                    // whether piece moved before
    int promoTo;                                        // piece promoted to
    ChessPiece captured;                                // piece that is captured by move

    // the board
    ChessPiece board[8][8];

public:
    Board();
    ~Board();
    void init();
    int value();
    bool validate(int start, int end);
    void switchToPlay();
    int getSpecial();
    bool gameOver();
    int getWinner();
    bool promote(int pieceNameVal);
    void goBack(int &start, int &end, int &special, int &promoPiece, int &capturedPiece, int &color);
    void goForward(int &start, int &end, int &special, int &promoPiece, int &capturedPiece, int &color);

private:
    bool pawnMoveValid(ChessPiece pawn, int startRow, int endRow, int startCol, int endCol);
    bool rookMoveValid(ChessPiece rook, int startRow, int endRow, int startCol, int endCol);
    bool knightMoveValid(ChessPiece knight, int startRow, int endRow, int startCol, int endCol);
    bool bishopMoveValid(ChessPiece bishop, int startRow, int endRow, int startCol, int endCol);
    bool queenMoveValid(ChessPiece queen, int startRow, int endRow, int startCol, int endCol);
    bool kingMoveValid(ChessPiece king, int startRow, int endRow, int startCol, int endCol);
    void movePiece(int startRow, int endRow, int startCol, int endCol);
    bool isChecked(int row, int col, ChessPiece cboard[8][8], int color, int rowList[], int colList[]);
    bool isMate(int row, int col, ChessPiece cboard[8][8], int rowList[], int colList[]);
    bool isPawnLos(int row, int col, ChessPiece cboard[8][8], int color);
    bool numToRowCol(int start, int end, int &startRow, int &endRow, int &startCol, int &endcol);
    int rowColToNum(int row, int col);
    bool inBounds(int row, int col);
    bool areEqual(int arr1[2], int arr2[2]);
    void assignArray(int arr[2], int val1, int val2);
    void addMove(int start, int end);
};
