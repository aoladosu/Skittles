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
    bool CHECK = false;                                 // a king in check
    bool DRAW = false;                                  // game is drawn
    bool INSUFFMAT = false;                             // insfuffienct material
    bool STALEMATE = false;                             // stalemate
    bool ENDGAME = false;                               // whether we are in end game
    short int toPlay = WHITE;							// what side is playing
    short int bkPos[2] = {0, 4}, wkPos[2] = {7, 4};		// king positions

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
    ChessPiece movedPiece;                              // piece that was moved

    // Error codes
    enum Error:short int {mate,                         // one side has won from check, game can't continue
                          draw,                         // game has been drawn, can't continue
                          promotion,                    // a pawn needs to be promoted
                          boundary,                     // the start or end position was out of bounds
                          color,                        // the wrong color is trying to play
                          kingInCheck,                  // the move is valid, but it leaves the king in check
                          pawnOntoPiece,                // pawn is trying to move onto a piece, but not diagonally for a capture
                          pawnDoubleSpace,              // pawn is trying to move forward by two, but it has already moved
                          selfCapture,                  // trying to capture own piece
                          enpassant,                    // trying to enpassant, but conditions not met
                          jump,                         // a piece is trying to jump over another
                          castle,                       // one or more of the castling rules was not met
                          badMovement,                  // piece not moved correctly
                          none                          // no errors, everything handles well
                         };
    Error ERROR;

    // the board
    ChessPiece board[8][8];

    // values for the pieces
    // gotten from: Chess Programming Wiki. https://www.chessprogramming.org/Simplified_Evaluation_Function
    short int pawnVals[64] ={ 0,  0,  0,  0,  0,  0,  0,  0,
                              50, 50, 50, 50, 50, 50, 50, 50,
                              10, 10, 20, 30, 30, 20, 10, 10,
                               5,  5, 10, 25, 25, 10,  5,  5,
                               0,  0,  0, 20, 20,  0,  0,  0,
                               5, -5,-10,  0,  0,-10, -5,  5,
                               5, 10, 10,-20,-20, 10, 10,  5,
                               0,  0,  0,  0,  0,  0,  0,  0};

    short int knightVals[64] ={ -50,-40,-30,-30,-30,-30,-40,-50,
                                -40,-20,  0,  0,  0,  0,-20,-40,
                                -30,  0, 10, 15, 15, 10,  0,-30,
                                -30,  5, 15, 20, 20, 15,  5,-30,
                                -30,  0, 15, 20, 20, 15,  0,-30,
                                -30,  5, 10, 15, 15, 10,  5,-30,
                                -40,-20,  0,  5,  5,  0,-20,-40,
                                -50,-40,-30,-30,-30,-30,-40,-50};

    short int bishopVals[64] ={ -20,-10,-10,-10,-10,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,  5, 10, 10,  5,  0,-10,
                                -10,  5,  5, 10, 10,  5,  5,-10,
                                -10,  0, 10, 10, 10, 10,  0,-10,
                                -10, 10, 10, 10, 10, 10, 10,-10,
                                -10,  5,  0,  0,  0,  0,  5,-10,
                                -20,-10,-10,-10,-10,-10,-10,-20};

    short int rookVals[64] = {  0,  0,  0,  0,  0,  0,  0,  0,
                                5, 10, 10, 10, 10, 10, 10,  5,
                               -5,  0,  0,  0,  0,  0,  0, -5,
                               -5,  0,  0,  0,  0,  0,  0, -5,
                               -5,  0,  0,  0,  0,  0,  0, -5,
                               -5,  0,  0,  0,  0,  0,  0, -5,
                               -5,  0,  0,  0,  0,  0,  0, -5,
                                0,  0,  0,  5,  5,  0,  0,  0};

    short int queenVals[64] = { -20,-10,-10, -5, -5,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,  5,  5,  5,  5,  0,-10,
                                 -5,  0,  5,  5,  5,  5,  0, -5,
                                  0,  0,  5,  5,  5,  5,  0, -5,
                                -10,  5,  5,  5,  5,  5,  0,-10,
                                -10,  0,  5,  0,  0,  0,  0,-10,
                                -20,-10,-10, -5, -5,-10,-10,-20};

    short int kingVals[64] = {-30,-40,-40,-50,-50,-40,-40,-30,
                              -30,-40,-40,-50,-50,-40,-40,-30,
                              -30,-40,-40,-50,-50,-40,-40,-30,
                              -30,-40,-40,-50,-50,-40,-40,-30,
                              -20,-30,-30,-40,-40,-30,-30,-20,
                              -10,-20,-20,-20,-20,-20,-20,-10,
                               20, 20,  0,  0,  0,  0, 20, 20,
                               20, 30, 10,  0,  0, 10, 30, 20};

    short int kingValsEnd[64] ={-50,-40,-30,-20,-20,-30,-40,-50,
                                -30,-20,-10,  0,  0,-10,-20,-30,
                                -30,-10, 20, 30, 30, 20,-10,-30,
                                -30,-10, 30, 40, 40, 30,-10,-30,
                                -30,-10, 30, 40, 40, 30,-10,-30,
                                -30,-10, 20, 30, 30, 20,-10,-30,
                                -30,-30,  0,  0,  0,  0,-30,-30,
                                -50,-30,-30,-30,-30,-30,-30,-50};

public:
    Board();
    ~Board();
    void init();
    short int value(ChessPiece cboard[8][8] = nullptr);
    bool validate(short int start, short int end);
    void switchToPlay();
    short int getSpecial();
    bool gameOver();
    short int getWinner();
    short int getWinReason();
    short int getErrorState();
    bool isStart();
    bool isEnd();
    bool promote(short int pieceNameVal);
    void goBack(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color);
    void goForward(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color);
    void moveStats(short int &pieceMoved, short int &color, bool &capture, bool &check, bool &checkmate);
    bool genMovesForPiece(short int pos, short int moves[], short int cMoves[], short int aMoves[]);
    void checkPositions(short int pos[]);

private:
    bool pawnMoveValid(ChessPiece pawn, short int startRow, short int endRow, short int startCol, short int endCol);
    bool rookMoveValid(ChessPiece rook, short int startRow, short int endRow, short int startCol, short int endCol);
    bool knightMoveValid(ChessPiece knight, short int startRow, short int endRow, short int startCol, short int endCol);
    bool bishopMoveValid(ChessPiece bishop, short int startRow, short int endRow, short int startCol, short int endCol);
    bool queenMoveValid(ChessPiece queen, short int startRow, short int endRow, short int startCol, short int endCol);
    bool kingMoveValid(ChessPiece king, short int startRow, short int endRow, short int startCol, short int endCol);
    bool isLeftInCheck(short int startRow, short int endRow, short int startCol, short int endCol, short int color, bool undo);
    void movePiece(short int startRow, short int endRow, short int startCol, short int endCol);
    bool isChecked(short int row, short int col, ChessPiece cboard[8][8], short int color, short int rowList[], short int colList[]);
    bool isMate(short int row, short int col, ChessPiece cboard[8][8], short int rowList[], short int colList[]);
    bool isDraw();
    bool isStalemate(short int pieces[]);
    bool genKingMoves(short int row, short int col, short int moves[]);
    bool genKnightMoves(short int row, short int col, short int moves[]);
    bool genBishopMoves(short int row, short int col, short int moves[]);
    bool genRookMoves(short int row, short int col, short int moves[]);
    bool genQueenMoves(short int row, short int col, short int moves[]);
    bool genPawnMoves(short int row, short int col, short int moves[]);
    bool isPawnLos(short int row, short int col, ChessPiece cboard[8][8], short int color);
    bool numToRowCol(short int start, short int end, short int &startRow, short int &endRow, short int &startCol, short int &endcol);
    short int rowColToNum(short int row, short int col);
    bool inBounds(short int row, short int col);
    bool areEqual(short int arr1[2], short int arr2[2]);
    void assignArray(short int arr[2], short int val1, short int val2);
    void addMove(short int start, short int end);
};
