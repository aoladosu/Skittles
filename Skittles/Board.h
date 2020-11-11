#pragma once
#include "ChessPiece.h"
#include "MoveList.h"

class Board
{
    // manage the board, by validating moves and moving pieces

private:

    // variables for piece type
    const short int BOARDSIZE = 8;
    const short int BLACK = 1;
    const short int WHITE = 0;
    const short int EMPTY = 0;
    const short int PAWN = 1;
    const short int ROOK = 2;
    const short int KNIGHT = 3;
    const short int BISHOP = 4;
    const short int QUEEN = 5;
    const short int KING = 6;

    // tracking enpassant variables
    short int enPassantPos[2] = {-2,-2};            // all manipulations will stay out of bounds
    short int enPassantColor = -1;
    short int enPassantPosOld[2] = { -2,-2 };       // stores the old cycle of enpassant
    short int enPassantColorOld = -1;

    // board state
    bool MATE = false;									// endgame conditions
    bool CHECK = false;
    short int toPlay = WHITE;							// what side is playing
    short int bkPos[2] = {0, 4}, wkPos[2] = {7, 4};		// king positions
    short int numPieces = 32;
    short int numPiecesOld = 32;

    // special moves
    const short int NOSPECIAL = 0;
    const short int PROMOTION = 1;
    const short int ENPASSANT = 2;
    const short int BQCASTLE = 3;
    const short int WQCASTLE = 4;
    const short int BKCASTLE = 5;
    const short int WKCASTLE = 6;
    short int specialMove = NOSPECIAL;
    short int promotionPos[2] = { -2, -2 };

    // variables for movelist
    MoveList moveList;                                  // store moves played
    bool movedStore;                                    // whether piece moved before
    short int promoTo;                                  // piece promoted to
    short int startPos;                                 // starting position of piece
    ChessPiece captured;                                // piece that is captured by move

    ChessPiece movedPiece;

    // the board
    ChessPiece board[8][8];

public:
    Board();
    ~Board();
    void init();
    int value();
    bool validate(short int start, short int end);
    void switchToPlay();
    short int getSpecial();
    bool gameOver();
    short int getWinner();
    bool promote(short int pieceNameVal);
    void goBack(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color);
    void goForward(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color);
    void moveStats(short int &pieceMoved, short int &color, bool &capture, bool &check, bool &checkmate);

private:
    bool pawnMoveValid(ChessPiece pawn, short int startRow, short int endRow, short int startCol, short int endCol);
    bool rookMoveValid(ChessPiece rook, short int startRow, short int endRow, short int startCol, short int endCol);
    bool knightMoveValid(ChessPiece knight, short int startRow, short int endRow, short int startCol, short int endCol);
    bool bishopMoveValid(ChessPiece bishop, short int startRow, short int endRow, short int startCol, short int endCol);
    bool queenMoveValid(ChessPiece queen, short int startRow, short int endRow, short int startCol, short int endCol);
    bool kingMoveValid(ChessPiece king, short int startRow, short int endRow, short int startCol, short int endCol);
    bool isLeftInCheck(short int startRow, short int endRow, short int startCol, short int endCol, bool undo);
    void movePiece(short int startRow, short int endRow, short int startCol, short int endCol);
    bool isChecked(short int row, short int col, ChessPiece cboard[8][8], short int color, short int rowList[], short int colList[], bool storeCheck=false);
    bool isMate(short int row, short int col, ChessPiece cboard[8][8], short int rowList[], short int colList[]);
    bool isPawnLos(short int row, short int col, ChessPiece cboard[8][8], short int color);
    bool numToRowCol(short int start, short int end, short int &startRow, short int &endRow, short int &startCol, short int &endcol);
    short int rowColToNum(short int row, short int col);
    bool inBounds(short int row, short int col);
    bool areEqual(short int arr1[2], short int arr2[2]);
    void assignArray(short int arr[2], short int val1, short int val2);
    void addMove(short int start, short int end);
};
