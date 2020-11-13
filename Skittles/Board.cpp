#include "Board.h"
#include <QtMath>

#include <QDebug>

Board::Board(){}
Board::~Board(){}

void Board::init()
{
    // initialize the board with a new game and place pieces on board

    short int blackVals[] = {50, 30, 30, 90, 900, 30, 30, 50};
    short int blackpawnVals = 10;
    short int blackNameVals[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
    short int whiteVals[] = {-50, -30, -30, -90, -900, -30, -30, -50};
    short int whitepawnVals = -10;
    short int whiteNameVals[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };

    for (short int i = 0; i < BOARDSIZE; i++) {
        board[0][i] = ChessPiece(blackVals[i], blackNameVals[i], BLACK);
        board[1][i] = ChessPiece(blackpawnVals, PAWN, BLACK);
        board[2][i] = ChessPiece();
        board[3][i] = ChessPiece();
        board[4][i] = ChessPiece();
        board[5][i] = ChessPiece();
        board[6][i] = ChessPiece(whitepawnVals, PAWN, WHITE);
        board[7][i] = ChessPiece(whiteVals[i], whiteNameVals[i], WHITE);
    }

    // (re)set variables
    MATE = false;
    DRAW = false;
    CHECK = false;
    INSUFFMAT = false;
    STALEMATE = false;
    specialMove = NOSPECIAL;
    assignArray(enPassantPos, -2, -2);             // all manipulations will stay out of bounds
    enPassantColor = -1;
    assignArray(enPassantPosOld, -2, -2);          // stores the old cycle of enpassant
    enPassantColorOld = -1;
    toPlay = WHITE;
    assignArray(bkPos, 0, 4);
    assignArray(wkPos, 7, 4);
    moveList.clear();
    captured = ChessPiece();
}

bool Board::validate(short int start, short int end)
{
    // check whether a move is valid and move piece if it is

    bool valid = false;

    if (MATE) return false;							// the game has been won
    if (DRAW) return false;                         // game is a draw
    if (specialMove == PROMOTION) return false;		// a pawn wasn't promoted, that needs to be done first
    specialMove = NOSPECIAL;						// reset to no special, it should be checked by front end after every turn ...

    short int startRow, endRow, startCol, endCol;
    if (!numToRowCol(start, end, startRow, endRow, startCol, endCol)) {
        return false;
    }

    ChessPiece piece = board[startRow][startCol], startPiece, endPiece, behindPiece;            // keep track of board state                                                                              // store if piece was moved
    short int rowList[16], colList[16], *currKing, *othKing, oppColor = WHITE + BLACK - toPlay;	// to check checkmate																// king's position

    if (piece.getColor() != toPlay) {
        return false;
    }

    if (toPlay == BLACK) {
        currKing = bkPos;
        othKing = wkPos;
    }
    else {
        currKing = wkPos;
        othKing = bkPos;
    }

    // store passant values
    assignArray(enPassantPosOld, enPassantPos[0], enPassantPos[1]);
    enPassantColorOld = enPassantColor;

    switch (piece.getNameValue()){
    case 1:
        valid = pawnMoveValid(piece, startRow, endRow, startCol, endCol);
        break;
    case 2:
        valid = rookMoveValid(piece, startRow, endRow, startCol, endCol);
        break;
    case 3:
        valid = knightMoveValid(piece, startRow, endRow, startCol, endCol);
        break;
    case 4:
        valid = bishopMoveValid(piece, startRow, endRow, startCol, endCol);
        break;
    case 5:
        valid = queenMoveValid(piece, startRow, endRow, startCol, endCol);
        break;
    case 6:
        valid = kingMoveValid(piece, startRow, endRow, startCol, endCol);
        break;
    default:
        valid = false;
    }

    if (valid) {

        if (isLeftInCheck(startRow, endRow, startCol, endCol, toPlay, false)) return false;

        // check if opposite king is in checkmate
        if (isChecked(othKing[0], othKing[1], board, oppColor, rowList, colList, true)) {
            if (isMate(othKing[0], othKing[1], board, rowList, colList)) {
                MATE = true;
            }
        }

        // check if game is a draw
        DRAW = isDraw();

        // update kings' positions
        if (piece.getNameValue() == KING) {
            currKing[0] = endRow;
            currKing[1] = endCol;
        }
        switchToPlay();
        short int temp[2] = { -2,-2 };
        if (!areEqual(enPassantPos,temp) && (enPassantColor != piece.getColor())) {
            assignArray(enPassantPos,temp[0],temp[1]);
            enPassantColor = -1;
        }

        // add move to movelist
        startPos = start;
        addMove(start, end);
    }

    return valid;
}

bool Board::pawnMoveValid(ChessPiece pawn, short int startRow, short int endRow, short int startCol, short int endCol)
{
    // validate pawn movement

    bool valid = false;

    short int end[2] = { endRow, endCol };
    short int color = pawn.getColor();
    short int row;
    short int pos1[2];
    short int pos2[2];
    short int pos3[2];
    short int pos4[2];

    if (color == BLACK) {
        assignArray(pos1, startRow + 1, startCol);			// 1 step forward
        assignArray(pos2, startRow + 2, startCol);			// 2 steps forward
        assignArray(pos3, startRow + 1, startCol + 1);		// capture right
        assignArray(pos4, startRow + 1, startCol - 1);		// capture left
        row = startRow + 1;
    }
    else {
        assignArray(pos1, startRow - 1, startCol);			// 1 step forward
        assignArray(pos2, startRow - 2, startCol);			// 2 steps forward
        assignArray(pos3, startRow - 1, startCol + 1);		// capture right
        assignArray(pos4, startRow - 1, startCol - 1);		// capture left
        row = startRow - 1;
    }

    ChessPiece piece = board[end[0]][end[1]];

    if (areEqual(end,pos1)) {
        // check no piece on this square
        valid = (piece.getNameValue() == EMPTY);
    }

    else if (areEqual(end,pos2)) {
        // check no pieces and pawn's first move
        if ((piece.getNameValue() == EMPTY) && (!pawn.getMoved()) && (board[row][end[1]].getNameValue() == EMPTY)) {
            enPassantColor = color;
            assignArray(enPassantPos,end[0],end[1]);
            valid = true;
        }
    }

    else if (areEqual(end,pos3) || areEqual(end,pos4)) {
        // check capture of other color
        if ((piece.getNameValue() != EMPTY) && (piece.getColor() != color)) {
            valid = true;
        }
        else if ((piece.getNameValue() == EMPTY) && (enPassantColor != color)) {

            if (color == BLACK) {
                if (((end[0] - 1) == enPassantPos[0]) && (end[1] == enPassantPos[1])){
                    specialMove = ENPASSANT;
                    valid = true;
                }
            }
            else {
                if (((end[0] + 1) == enPassantPos[0]) && (end[1] == enPassantPos[1])) {
                    specialMove = ENPASSANT;
                    valid = true;
                }
            }
        }
    }

    if ((valid) && ((endRow == 0) || (endRow == (BOARDSIZE - 1)))) {
        specialMove = PROMOTION;
        assignArray(promotionPos, endRow, endCol);
    }

    return valid;
}

bool Board::rookMoveValid(ChessPiece rook, short int startRow, short int endRow, short int startCol, short int endCol)
{
    // validate rook movements
    // castling is done in king verification

    bool valid = false;
    short int rowShift = (short int)(startRow != endRow);
    short int colShift = (short int)(startCol != endCol);

    if (rowShift == colShift) {
        // xor. either diagonalish or stayed in the same spot
        return valid;
    }

    if (endCol < startCol) {
        colShift = -1;
    }

    if (endRow < startRow) {
        rowShift = -1;
    }

    for (short int row = startRow + rowShift, col = startCol + colShift; ((row != endRow) || (col != endCol)); row += rowShift, col += colShift) {
        // check there are no pieces in the way
        if (board[row][col].getNameValue() != EMPTY) {
            return valid;
        }
    }

    if (board[endRow][endCol].getColor() == rook.getColor()) {
        // trying to capture own piece
        return valid;
    }

    // valid move
    valid = true;
    return valid;
}

bool Board::knightMoveValid(ChessPiece knight, short int startRow, short int endRow, short int startCol, short int endCol)
{
    // validate knight movements

    bool valid = false;

    short int pos1[] = { static_cast<short>(startRow - 2), static_cast<short>(startCol + 1) };	// 2up 1right
    short int pos2[] = { static_cast<short>(startRow - 1), static_cast<short>(startCol + 2) };	// 1up 2right
    short int pos3[] = { static_cast<short>(startRow + 1), static_cast<short>(startCol + 2) };	// 1down 2right
    short int pos4[] = { static_cast<short>(startRow + 2), static_cast<short>(startCol + 1) };	// 2down 1right
    short int pos5[] = { static_cast<short>(startRow + 2), static_cast<short>(startCol - 1) };	// 2down 1left
    short int pos6[] = { static_cast<short>(startRow + 1), static_cast<short>(startCol - 2) };	// 1down 2left
    short int pos7[] = { static_cast<short>(startRow - 1), static_cast<short>(startCol - 2) };	// 1up 2left
    short int pos8[] = { static_cast<short>(startRow - 2), static_cast<short>(startCol - 1) };	// 2up 1left
    short int end[] = { endRow, endCol };

    if (knight.getColor() == board[endRow][endCol].getColor()) {
        // can't capture own piece
        valid = false;
    }
    else if (areEqual(end,pos1) || areEqual(end,pos2) || areEqual(end,pos3) || areEqual(end,pos4) || areEqual(end,pos5) || areEqual(end,pos6) || areEqual(end,pos7) || areEqual(end,pos8)){
        valid = true;
    }
    return valid;
}

bool Board::bishopMoveValid(ChessPiece bishop, short int startRow, short int endRow, short int startCol, short int endCol)
{
    // validate bishop moves

    bool valid = false;
    short int rowShift = 1;
    short int colShift = 1;

    if (qFabs(endRow - startRow) != qFabs(endCol - startCol)) {
        // didn't move diagonally
        return valid;
    }

    if (endCol < startCol) {
        colShift = -1;
    }

    if (endRow < startRow) {
        rowShift = -1;
    }

    for (short int row = startRow + rowShift, col = startCol + colShift; ((row != endRow) || (col != endCol)); row += rowShift, col += colShift) {
        // check there are no pieces in the way
        if (board[row][col].getNameValue() != EMPTY) {
            return valid;
        }
    }

    if (board[endRow][endCol].getColor() == bishop.getColor()) {
        // trying to capture own piece
        return valid;
    }

    // valid move
    valid = true;
    return valid;
}

bool Board::queenMoveValid(ChessPiece queen, short int startRow, short int endRow, short int startCol, short int endCol)
{
    // validate queen movement
    // queen movement is a combination of bishop and rook movement

    return ((bishopMoveValid(queen, startRow, endRow, startCol, endCol)) || (rookMoveValid(queen, startRow, endRow, startCol, endCol)));
}

bool Board::kingMoveValid(ChessPiece king, short int startRow, short int endRow, short int startCol, short int endCol)
{
    // validate king movements
    // castling rules: king/rook can't have moved, nothing between king/rook, king not in check and
    // does not pass/stop on square attacked by enemy

    bool valid = false;
    short int rList[16], cList[16];

    short int pos1[2] = { static_cast<short>(startRow - 1), static_cast<short>(startCol + 0) };	// 1up 0right
    short int pos2[2] = { static_cast<short>(startRow - 1), static_cast<short>(startCol + 1) };	// 1up 1right
    short int pos3[2] = { static_cast<short>(startRow + 0), static_cast<short>(startCol + 1) };	// 0down 1right
    short int pos4[2] = { static_cast<short>(startRow + 1), static_cast<short>(startCol + 1) };	// 1down 1right
    short int pos5[2] = { static_cast<short>(startRow + 1), static_cast<short>(startCol - 0) };	// 1down 0left
    short int pos6[2] = { static_cast<short>(startRow + 1), static_cast<short>(startCol - 1) };	// 1down 1left
    short int pos7[2] = { static_cast<short>(startRow - 0), static_cast<short>(startCol - 1) };	// 0up 1left
    short int pos8[2] = { static_cast<short>(startRow - 1), static_cast<short>(startCol - 1) };	// 1up 1left
    short int pos9[2] = { startRow, static_cast<short>(startCol + 2) };	    // king side castle
    short int pos10[2] = { startRow, static_cast<short>(startCol - 2) };	    // queen side castle
    short int end[2] = { endRow, endCol };

    if (king.getColor() == board[endRow][endCol].getColor()) {
        // can't capture own piece
        valid = false;
    }
    else if (areEqual(end,pos1) || areEqual(end,pos2) || areEqual(end,pos3) || areEqual(end,pos4) || areEqual(end,pos5) || areEqual(end,pos6) || areEqual(end,pos7) || areEqual(end,pos8)) {
        // regular movement
        valid = true;
    }

    else if (areEqual(end,pos9)) {
        // king side castle -> to right
        // verify the king is not in check nor the squares it passes over
        if (!isChecked(startRow, startCol, board, king.getColor(), rList, cList) && !king.getMoved() && (board[endRow][endCol-1].getNameValue() == EMPTY) && (board[endRow][endCol].getNameValue() == EMPTY) && (!board[endRow][endCol+1].getMoved()) && (board[endRow][endCol+1].getNameValue() == ROOK) && !isChecked(endRow, endCol - 1, board, king.getColor(), rList, cList) && !isChecked(endRow, endCol, board, king.getColor(), rList, cList)) {
            valid = true;
            if (king.getColor() == BLACK) {
                specialMove = BKCASTLE;
            }
            else {
                specialMove = WKCASTLE;
            }
        }
    }

    else if (areEqual(end,pos10)) {
        // queen side castle -> to left
        // verify the king is not in check nor the squares it passes over
        if (!isChecked(startRow, startCol, board, king.getColor(), rList, cList) && !king.getMoved() && (board[endRow][endCol+1].getNameValue() == EMPTY) && (board[endRow][endCol].getNameValue() == EMPTY) && (board[endRow][endCol-1].getNameValue() == EMPTY) && (board[endRow][endCol-2].getNameValue() == ROOK) && (!board[endRow][endCol-2].getMoved()) && !isChecked(endRow, endCol, board, king.getColor(), rList, cList) && !isChecked(endRow, endCol + 1, board, king.getColor(), rList, cList)) {
            valid = true;
            if (king.getColor() == BLACK) {
                specialMove = BQCASTLE;
            }
            else {
                specialMove = WQCASTLE;
            }
        }
    }

    return valid;
}

void Board::movePiece(short int startRow, short int endRow, short int startCol, short int endCol)
{
    // move the chess piece to the new location

    ChessPiece piece = board[startRow][startCol];
    movedPiece = piece;
    movedStore = piece.getMoved();
    piece.setMoved(true);
    captured = board[endRow][endCol];
    board[endRow][endCol] = piece;
    board[startRow][startCol] = ChessPiece();

    // en passant, castling,
    if (specialMove == ENPASSANT) {
        captured = board[startRow][endCol];
        board[startRow][endCol] = ChessPiece();
    }
    else if ((specialMove == BQCASTLE) || (specialMove == WQCASTLE)) {
        // queen side castle -> to left
        board[endRow][endCol + 1] = board[endRow][endCol - 2];
        board[endRow][endCol + 1].setMoved(true);
        board[endRow][endCol - 2] = ChessPiece();
    }
    else if ((specialMove == BKCASTLE) || (specialMove == WKCASTLE)){
        // king side castle -> to right
        board[endRow][endCol - 1] = board[endRow][endCol + 1];
        board[endRow][endCol - 1].setMoved(true);
        board[endRow][endCol + 1] = ChessPiece();
    }
    return;
}

bool Board::isLeftInCheck(short int startRow, short int endRow, short int startCol, short int endCol, short int color, bool undo){
    // check if moving a piece leaves the king in check

    ChessPiece startPiece, endPiece, behindPiece;
    bool moved, inCheck=false;
    short int kingRow, kingCol, *currKing, *othKing, rowList[16], colList[16];

    // store past state of board
    startPiece = board[startRow][startCol];
    moved = startPiece.getMoved();
    endPiece = board[endRow][endCol];
    ChessPiece movedPieceOld = movedPiece;
    ChessPiece capturedOld = captured;
    if (startPiece.getNameValue() == PAWN) {
        if (startPiece.getColor() == BLACK) behindPiece = board[endRow - 1][endCol];
        else behindPiece = board[endRow + 1][endCol];
    }

    // move piece
    movePiece(startRow, endRow, startCol, endCol);

    // get king's position, might have changed if the king was moved
    if (color == BLACK) {
        currKing = bkPos;
        othKing = wkPos;
    }
    else {
        currKing = wkPos;
        othKing = bkPos;
    }
    if (startPiece.getNameValue() == KING) {
        kingRow = endRow;
        kingCol = endCol;
    }
    else {
        kingRow = currKing[0];
        kingCol = currKing[1];
    }

    // check if move causes this king to be in check
    if (isChecked(kingRow, kingCol, board, color, rowList, colList)) {
        undo = true;
        inCheck = true;
    }

    if (undo){
        // undo move
        if (startPiece.getNameValue() == PAWN) {
            if (startPiece.getColor() == BLACK) board[endRow - 1][endCol] = behindPiece;
            else board[endRow + 1][endCol] = behindPiece;
        }
        board[startRow][startCol] = startPiece;
        board[startRow][startCol].setMoved(moved);
        board[endRow][endCol] = endPiece;
        specialMove = NOSPECIAL;
        assignArray(enPassantPos, enPassantPosOld[0], enPassantPosOld[1]);
        enPassantColor = enPassantColorOld;
        captured = capturedOld;
        movedPiece = movedPieceOld;
        movedStore = movedPieceOld.getMoved();
    }

    return inCheck;

}

bool Board::isChecked(short int row, short int col, ChessPiece cboard[8][8], short int color, short int rowList[], short int colList[], bool storeCheck) {

    // check if the given row/col is attacked by a piece
    // color is the the perspective to look from in case row/col refers to an empty square
    // that is color is color of the piece that is (or would be) occupying the square

    if (!inBounds(row, col)) return false;

    ChessPiece piece;
    short int index = 0;

    // pawn positions
    short int prowPos;
    if (color == BLACK) {
        prowPos = row + 1;
    }
    else {
        prowPos = row - 1;
    }
    int pcolPos[2] = { col + 1, col - 1 };

    // check if pawn is attacking row,col
    for (short int i = 0; i < 2; i++) {
        if (inBounds(prowPos, pcolPos[i])) {
            if ((cboard[prowPos][pcolPos[i]].getNameValue() == PAWN) && (cboard[prowPos][pcolPos[i]].getColor() != color)){
                rowList[index] = prowPos;
                colList[index] = pcolPos[i];
                index++;
            }
        }
    }

    // checks if the piece (pawn) in row/col can be attacked by enpassant
    if (inBounds(row, col - 1) && (row == enPassantPos[0]) && (col == enPassantPos[1])) {
        piece = cboard[row][col - 1];
        if ((piece.getNameValue() == PAWN) && (piece.getColor() != color)){
            rowList[index] = row;
            colList[index] = col-1;
            index++;
        }
    }
    if (inBounds(row, col + 1) && (row == enPassantPos[0]) && (col == enPassantPos[1])) {
        piece = cboard[row][col + 1];
        if ((piece.getNameValue() == PAWN) && (piece.getColor() != color)) {
            rowList[index] = row;
            colList[index] = col + 1;
            index++;
        }
    }

    /*
    std::array<int, 2> kpos1 = { row - 2, col + 1 };	// 2up 1right
    std::array<int, 2> kpos2 = { row - 1, col + 2 };	// 1up 2right
    std::array<int, 2> kpos3 = { row + 1, col + 2 };	// 1down 2right
    std::array<int, 2> kpos4 = { row + 2, col + 1 };	// 2down 1right
    std::array<int, 2> kpos5 = { row + 2, col - 1 };	// 2down 1left
    std::array<int, 2> kpos6 = { row + 1, col - 2 };	// 1down 2left
    std::array<int, 2> kpos7 = { row - 1, col - 2 };	// 1up 2left
    std::array<int, 2> kpos8 = { row - 2, col - 1 };	// 2up 1left
    */

    // knight positions
    short int krowPos[8] = { static_cast<short>(row - 2), static_cast<short>(row - 1), static_cast<short>(row + 1), static_cast<short>(row + 2), static_cast<short>(row + 2), static_cast<short>(row + 1), static_cast<short>(row - 1), static_cast<short>(row - 2) };
    short int kcolPos[8] = { static_cast<short>(col + 1), static_cast<short>(col + 2), static_cast<short>(col + 2), static_cast<short>(col + 1), static_cast<short>(col - 1), static_cast<short>(col - 2), static_cast<short>(col - 2), static_cast<short>(col - 1) };
    // check if knight is attacking row,col
    for (short int i=0; i<BOARDSIZE; i++){
        if (inBounds(krowPos[i], kcolPos[i]) == false) continue;
        piece = cboard[krowPos[i]][kcolPos[i]];
        if ((piece.getNameValue() == KNIGHT) && (piece.getColor() != color)) {
            rowList[index] = krowPos[i];
            colList[index] = kcolPos[i];
            index++;
        }
    }

    // LOS check for rook,bishop,queen and king

    // king is an 'exception' in that it needs to be one space away so just hard check for it
    for (short int i = row - 1; i < row + 2; i++) {
        for (short int j = col - 1; j < col + 2; j++) {
            if (inBounds(i, j)) {
                piece = cboard[i][j];
                if ((piece.getNameValue() == KING) && (piece.getColor() != color)) {
                    rowList[index] = i;
                    colList[index] = j;
                    index++;
                }
            }
        }
    }

    // check if there is a queen or rook upward
    for (short int i = row - 1, j = col; i >= 0; i--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or rook downward
    for (short int i = row + 1, j = col; i < BOARDSIZE; i++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or rook leftward
    for (short int i = row, j = col-1; j >=0; j--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or rook rightward
    for (short int i = row, j = col + 1; j < BOARDSIZE; j++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop up-rightward
    for (short int i = row - 1, j = col + 1; ((i >= 0) && (j < BOARDSIZE)); i--, j++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop up-leftward
    for (short int i = row - 1, j = col - 1; ((i >= 0) && (j >= 0)); i--, j--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop down-rightward
    for (short int i = row + 1, j = col + 1; ((i < BOARDSIZE) && (j < BOARDSIZE)); i++, j++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop down-leftward
    for (short int i = row + 1, j = col - 1; ((i < BOARDSIZE) && (j >= 0)); i++, j--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    rowList[index] = -1;
    colList[index] = -1;
    if (storeCheck) CHECK = (index != 0);
    return (index != 0);
}

bool Board::isMate(short int row, short int col, ChessPiece cboard[8][8], short int rowList[], short int colList[]) {
    // check if the king at row/col is in check mate, assumes the king being in check has been checked

    if (cboard[row][col].getNameValue() != KING) return false;
    short int rowInfo[16], colInfo[16];
    short int color = cboard[row][col].getColor();
    short int oppColor = WHITE + BLACK - color;
    short int i = -1;
    while (rowList[++i] != -1);
    ChessPiece king = cboard[row][col], piece, blankPiece = ChessPiece();

    // if 1 piece attacking, run it through isChecked to see if there is a piece that can capture it
    // if king can capture piece, make sure king is not moving into check still
    if (i == 1) {
        if (isChecked(rowList[0], colList[0], cboard, oppColor, rowInfo, colInfo)) {
            if (!((rowInfo[1] == -1) && (rowInfo[0] == row) && (colInfo[0] == col) )) {
                return false;
            }
            // modify cboard
            piece = cboard[rowList[0]][colList[0]];
            cboard[rowList[0]][colList[0]] = king;
            cboard[row][col] = blankPiece;
            if (!isChecked(rowList[0], colList[0], cboard, color, rowInfo, colInfo)) {
                // king isn't moving into check, undo modify board
                cboard[rowList[0]][colList[0]] = piece;
                cboard[row][col] = king;
                return false;
            }
            // undo modify board
            cboard[rowList[0]][colList[0]] = piece;
            cboard[row][col] = king;
        }
    }

    // else or if there are 2 pieces, see if the king can move out of check
    cboard[row][col] = blankPiece;
    for (short int j = row - 1; j < row + 2; j++) {
        for (short int k = col - 1; k < col + 2; k++) {
            if (inBounds(j, k)) {
                // modify cboard, move king
                piece = cboard[j][k];
                cboard[j][k] = king;
                if (!isChecked(j, k, cboard, oppColor, rowInfo, colInfo)) {
                    // undo modify board
                    cboard[j][k] = piece;
                    cboard[row][col] = king;
                    return false;
                }
                // undo modify board
                cboard[j][k] = piece;
            }
        }
    }
    cboard[row][col] = king;

    if ((i == 2) || (cboard[rowList[0]][colList[0]].getNameValue() == KNIGHT) || (cboard[rowList[1]][colList[1]].getNameValue() == KNIGHT)) {
        // with a 2piece check if the king can't move, then it is check mate, or a knight can't be los'd
        return true;
    }

    // else see if a piece can block los
    short int startRow = row;
    short int startCol = col;
    short int endRow = rowList[0];
    short int endCol = colList[0];
    short int rowShift, colShift;

    // set up iteration variables
    if (row == endRow){
        rowShift = 0;
    }
    else {
        if (row > endRow){
            rowShift = -1;
        }
        else{
            rowShift = 1;
        }
    }
    if (col == endCol){
        colShift = 0;
    }
    else {
        if (col > endCol){
            colShift = -1;
        }
        else{
            colShift = 1;
        }
    }

    for (short int rowIndex = startRow + rowShift, colIndex = startCol + colShift; ((rowIndex != endRow) || (colIndex != endCol)); rowIndex += rowShift, colIndex += colShift) {
        // check if a piece can move to the current space
        if (isPawnLos(rowIndex, colIndex, cboard, oppColor)) {
            return false;
        }
        if (isChecked(rowIndex, colIndex, cboard, oppColor, rowInfo, colInfo)) {
            if (!((rowInfo[1] == -1) && (rowInfo[0] == row) && (colInfo[0] == col))) {
                // check king isn't blocking own los
                return false;
            }
        }
    }

    return true;
}

bool Board::isDraw(){

    // variables
    short int bPieces[17], bIndex=0;
    short int wPieces[17], wIndex=0;
    ChessPiece piece;

    // get where pieces are
    for(short int i=0; i<BOARDSIZE; i++){
        for (short int j=0; j<BOARDSIZE; j++){
            piece = board[i][j];
            if (piece.getColor() == BLACK){
                bPieces[bIndex++] = rowColToNum(i, j);
            }
            else{
                wPieces[wIndex++] = rowColToNum(i, j);
            }
        }
    }

    // check for insufficient material
    if ((bIndex < 3) || (wIndex < 3)){
        bool bInsuff=false, wInsuff=false;
        short int row, col, row1, col1, p1,p2,p3;

        // black pieces
        if (bIndex == 1) bInsuff = true;
        else if (bIndex == 2){
            // king + bishop/knight
            numToRowCol(bPieces[0], 0, row, col, row1, col1);
            bInsuff = ((board[row][col].getNameValue() == BISHOP) || (board[row][col].getNameValue() == KNIGHT));
            numToRowCol(bPieces[1], 0, row, col, row1, col1);
            bInsuff = (bInsuff || ((board[row][col].getNameValue() == BISHOP) || (board[row][col].getNameValue() == KNIGHT)));
        }
        else if (bIndex == 3){
            // king + 2 Knights
            numToRowCol(bPieces[0], 0, row, col, row1, col1);
            p1 = board[row][col].getNameValue() == KNIGHT;
            numToRowCol(bPieces[1], 0, row, col, row1, col1);
            p2 = board[row][col].getNameValue() == KNIGHT;
            numToRowCol(bPieces[2], 0, row, col, row1, col1);
            p3 = board[row][col].getNameValue() == KNIGHT;
            bInsuff = (p1 + p2 + p3 == 2);
        }

        // white pieces
        if (wIndex == 1) wInsuff = true;
        else if (wIndex == 2){
            // king + bishop/knight
            numToRowCol(wPieces[0], 0, row, col, row1, col1);
            wInsuff = ((board[row][col].getNameValue() == BISHOP) || (board[row][col].getNameValue() == KNIGHT));
            numToRowCol(wPieces[1], 0, row, col, row1, col1);
            wInsuff = (wInsuff || ((board[row][col].getNameValue() == BISHOP) || (board[row][col].getNameValue() == KNIGHT)));
        }
        else if (wIndex == 3){
            // king + 2 Knights
            numToRowCol(wPieces[0], 0, row, col, row1, col1);
            p1 = board[row][col].getNameValue() == KNIGHT;
            numToRowCol(wPieces[1], 0, row, col, row1, col1);
            p2 = board[row][col].getNameValue() == KNIGHT;
            numToRowCol(wPieces[2], 0, row, col, row1, col1);
            p3 = board[row][col].getNameValue() == KNIGHT;
            wInsuff = ((p1 + p2 + p3) == 2);
        }

        if (bInsuff && wInsuff){
            INSUFFMAT = true;
            return true;
        }
    }


    // check for stalemate
    bPieces[bIndex++] = -1;
    wPieces[wIndex++] = -1;
    //if (toPlay == BLACK) STALEMATE = isStalemate(bPieces);
    //else STALEMATE = isStalemate(wPieces);
    return INSUFFMAT;

}

bool Board::isStalemate(short int pieces[]){
    // check for stalemate

    short int row, col, row1, col1, index=0, moves[35];
    bool canMove = false;

    while (pieces[index] != -1){
        numToRowCol(pieces[index], 0, row, col, row1, col1);
        switch (board[row][col].getNameValue()) {
            case 1:
                canMove = genPawnMoves(row, col, moves);
                break;
            case 2:
                canMove = genRookMoves(row, col, moves);
                break;
            case 3:
                canMove = genKnightMoves(row, col, moves);
                break;
            case 4:
                canMove = genBishopMoves(row, col, moves);
                break;
            case 5:
                canMove = genQueenMoves(row, col, moves);
                break;
            case 6:
                canMove = genKingMoves(row, col, moves);
                break;
        }
        index += 1;
        if (canMove) return false;
    }

    return true;
}

bool Board::genKingMoves(short int row, short int col, short int moves[]){
    // generate the moves for the king

    if (board[row][col].getNameValue() != KING) return false;
    short int special = specialMove, index=0;
    ChessPiece piece = board[row][col];
    short int color = piece.getColor();

    // check surrounding eightpositions
    for (short int i = row -1; i < row+2; i++){
        for (short int j= col -1; j < col+2; j++){
            if(inBounds(i,j)){
                if (kingMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)){
                    moves[index++] = rowColToNum(i,j);
                }
            }
        }
    }


    // check castling to right
    if(inBounds(row,col+2)){
        if (kingMoveValid(piece, row, row, col, col+2) && !isLeftInCheck(row, row, col, col+2, color, true)) moves[index++] = rowColToNum(row,col+2);
    }

    // check castling to left
    if(inBounds(row,col-2)){
        if (kingMoveValid(piece, row, row, col, col-2) && !isLeftInCheck(row, row, col, col-2, color, true)) moves[index++] = rowColToNum(row,col-2);
    }

    moves[index] = -1;
    specialMove = special;
    return (index != 0);

}

bool Board::genKnightMoves(short int row, short int col, short int moves[]){
    // generate moves for knight

    if (board[row][col].getNameValue() != KNIGHT) return false;
    short int special = specialMove, index=0;
    ChessPiece piece = board[row][col];
    short int color = piece.getColor();

    // knight moves
    short int krowPos[8] = { static_cast<short>(row - 2), static_cast<short>(row - 1), static_cast<short>(row + 1), static_cast<short>(row + 2), static_cast<short>(row + 2), static_cast<short>(row + 1), static_cast<short>(row - 1), static_cast<short>(row - 2) };
    short int kcolPos[8] = { static_cast<short>(col + 1), static_cast<short>(col + 2), static_cast<short>(col + 2), static_cast<short>(col + 1), static_cast<short>(col - 1), static_cast<short>(col - 2), static_cast<short>(col - 2), static_cast<short>(col - 1) };

    // check if knight is moves valid
    for (short int i=0; i<BOARDSIZE; i++){
        if (inBounds(krowPos[i], kcolPos[i]) == false) continue;
        if (knightMoveValid(piece, row, krowPos[i], col, kcolPos[i]) && !isLeftInCheck(row, krowPos[i], col, kcolPos[i], color, true)) moves[index++] = rowColToNum(krowPos[i],kcolPos[i]);
    }

    moves[index] = -1;
    specialMove = special;
    return (index != 0);
}

bool Board::genBishopMoves(short int row, short int col, short int moves[]){
    // generate moves for bishop

    if ((board[row][col].getNameValue() != BISHOP) && (board[row][col].getNameValue() != QUEEN)) return false;
    short int special = specialMove, index=0;
    ChessPiece piece = board[row][col];
    short int color = piece.getColor();

    // check up left direction
    for (short int i=row-1, j=col-1; inBounds(i, j); i-=1, j-=1){
        if (bishopMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    // check up right direction
    for (short int i=row-1, j=col+1; inBounds(i, j); i-=1, j+=1){
        if (bishopMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    // check down left direction
    for (short int i=row+1, j=col-1; inBounds(i, j); i+=1, j-=1){
        if (bishopMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    // check down right direction
    for (short int i=row+1, j=col+1; inBounds(i, j); i+=1, j+=1){
        if (bishopMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    moves[index] = -1;
    specialMove = special;
    return (index != 0);
}

bool Board::genRookMoves(short int row, short int col, short int moves[]){
    // generate moves for rook

    if ((board[row][col].getNameValue() != ROOK) && (board[row][col].getNameValue() != QUEEN)) return false;
    short int special = specialMove, index=0;
    ChessPiece piece = board[row][col];
    short int color = piece.getColor();

    // check up direction
    for (short int i=row-1, j=col; inBounds(i, j); i-=1){
        if (rookMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    // check down direction
    for (short int i=row+1, j=col; inBounds(i, j); i+=1){
        if (rookMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    // check left direction
    for (short int i=row, j=col-1; inBounds(i, j); j-=1){
        if (rookMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    // check right direction
    for (short int i=row, j=col+1; inBounds(i, j); j+=1){
        if (rookMoveValid(piece, row, i, col, j) && !isLeftInCheck(row, i, col, j, color, true)) moves[index++] = rowColToNum(i,j);
        else break;
    }

    moves[index] = -1;
    specialMove = special;
    return (index != 0);
}

bool Board::genQueenMoves(short int row, short int col, short int moves[]){
    // generate moves for queen

    short int index=0;
    bool areMoves = false;
    areMoves = genBishopMoves(row,col,moves);
    while (moves[index++] != -1);
    areMoves = genRookMoves(row,col, moves+(index-1)) || areMoves;
    return areMoves;
}

bool Board::genPawnMoves(short int row, short int col, short int moves[]){
    // gen moves for pawn

    if (board[row][col].getNameValue() != PAWN) return false;
    short int special = specialMove, index=0, enPassantColor2 = enPassantColor;
    short int promotionPos2[2], enPassantPos2[2];
    ChessPiece piece = board[row][col];
    short int color = piece.getColor();

    // store value in order to restore later
    assignArray(promotionPos2, promotionPos[0], promotionPos[1]);
    assignArray(enPassantPos2, enPassantPos[0], enPassantPos[1]);

    // pawn moves
    short int prowPos[8] = { static_cast<short>(row - 1), static_cast<short>(row + 1), static_cast<short>(row - 2), static_cast<short>(row + 2), static_cast<short>(row - 1), static_cast<short>(row + 1), static_cast<short>(row - 1), static_cast<short>(row + 1) };
    short int pcolPos[8] = { static_cast<short>(col), static_cast<short>(col), static_cast<short>(col), static_cast<short>(col), static_cast<short>(col + 1), static_cast<short>(col + 1), static_cast<short>(col - 1), static_cast<short>(col - 1) };


    for (short int i =0, j,k; i<8; i++){
        j = prowPos[i];
        k = pcolPos[i];
        if (inBounds(j, k)){
            // black piece
            if (pawnMoveValid(piece, row, j, col, k) && !isLeftInCheck(row, j, col, k, color, true)){
                moves[index++] = rowColToNum(j,k);
                enPassantColor = enPassantColor2;
                assignArray(enPassantPos, enPassantPos2[0], enPassantPos2[1]);
            }
        }
    }


    moves[index] = -1;
    specialMove = special;
    enPassantColor = enPassantColor2;
    assignArray(promotionPos, promotionPos2[0], promotionPos2[1]);
    assignArray(enPassantPos, enPassantPos2[0], enPassantPos2[1]);
    return (index != 0);

}

bool Board::isPawnLos(short int row, short int col, ChessPiece cboard[8][8], short int color){
    // check if a pawn can be moved on this square

    if (cboard[row][col].getNameValue() != EMPTY) return false;

    if (color == BLACK) {
        if (inBounds(row + 1, col) && (cboard[row+1][col].getNameValue() == PAWN) && (cboard[row+1][col].getColor() == WHITE)) {
            return true;
        }
        if (inBounds(row + 2, col) && (cboard[row + 1][col].getNameValue() == EMPTY) && (cboard[row + 2][col].getNameValue() == PAWN) && (cboard[row + 2][col].getMoved() == false)){
            return true;
        }
        return false;
    }
    else {
        if (inBounds(row - 1, col) && (cboard[row - 1][col].getNameValue() == PAWN) && (cboard[row - 1][col].getColor() == BLACK)) {
            return true;
        }
        if (inBounds(row - 2, col) && (cboard[row - 1][col].getNameValue() == EMPTY) && (cboard[row - 2][col].getNameValue() == PAWN) && (cboard[row - 2][col].getMoved() == false)) {
            return true;
        }
        return false;
    }
}

void Board::goBack(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color)
{
    // go forward a move

    Move *move = moveList.getPrevious();
    if (move == nullptr){
        start = -1;
        end = -1;
        special = -1;
        promoPiece = -1;
        capturedPiece = -1;
        color = -1;
        return;
    }

    // get correct enPassant values
    Move *move2 = moveList.getPrevious();
    if (move2 != nullptr){
        moveList.getNext();
    }

    // set variables to send back
    start = move->start;
    end = move->end;
    special = move->specialMove;
    promoPiece = move->promoTo;
    ChessPiece captPiece = ChessPiece(move->piece);
    capturedPiece = captPiece.getNameValue();

    // set board variables back
    if (move2 != nullptr){
        assignArray(enPassantPos, move2->enPassant[0], move2->enPassant[1]);
        enPassantColor = move2->enPassantColor;
    }
    else{
        assignArray(enPassantPos, -2, -2);
        enPassantColor = -1;
    }

    specialMove = NOSPECIAL;
    MATE = move->mate;
    CHECK = move->check;

    // set pieces including captured back
    short int startRow, endRow, startCol, endCol, specialmv;
    specialmv = move->specialMove;
    numToRowCol(start, end, startRow, endRow, startCol, endCol);
    board[startRow][startCol] = board[endRow][endCol];
    board[startRow][startCol].setMoved(move->moved);
    movedPiece = board[startRow][startCol];
    captured = ChessPiece();

    if (specialmv == ENPASSANT){
        if (board[startRow][startCol].getColor() == BLACK){
            board[endRow-1][endCol] = captPiece;
        }
        else{
            board[endRow+1][endCol] = captPiece;
        }
        board[endRow][endCol] = ChessPiece();
    }
    else{
        board[endRow][endCol] = captPiece;
    }

    // handle special cases
    if (specialmv == PROMOTION){
        board[startRow][startCol] = ChessPiece(0, PAWN, board[startRow][startCol].getColor());
        board[startRow][startCol].setMoved(true);
    }
    else if (specialmv == BQCASTLE){
        board[0][0] = board[0][3];
        board[0][0].setMoved(false);
        board[0][3] = ChessPiece();
    }
    else if (specialmv == BKCASTLE){
        board[0][7] = board[0][5];
        board[0][7].setMoved(false);
        board[0][5] = ChessPiece();
    }
    else if (specialmv == WQCASTLE){
        board[7][0] = board[7][3];
        board[7][0].setMoved(false);
        board[7][3] = ChessPiece();
    }
    else if (specialmv == WKCASTLE){
        board[7][7] = board[7][5];
        board[7][7].setMoved(false);
        board[7][5] = ChessPiece();
    }

    toPlay = board[startRow][startCol].getColor();
    color = toPlay;

    if (board[startRow][startCol].getNameValue() == KING){
        // if the piece moved is a king update king pos
        if (color == BLACK) assignArray(bkPos, startRow, startCol);
        else assignArray(wkPos, startRow, startCol);
    }
}

void Board::goForward(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color){
    // go foraward a move

    Move *move = moveList.getNext();
    if (move == nullptr){
        start = -1;
        end = -1;
        special = -1;
        promoPiece = -1;
        capturedPiece = -1;
        return;
    }

    // set variables to send back
    start = move->start;
    end = move->end;
    special = move->specialMove;
    promoPiece = move->promoTo;
    capturedPiece = -1;

    // set variables for board
    assignArray(enPassantPos, move->enPassant[0], move->enPassant[1]);
    enPassantColor = move->enPassantColor;
    MATE = move->mate;
    CHECK = move->check;
    short int startRow, endRow, startCol, endCol;
    specialMove = move->specialMove;
    if (specialMove == PROMOTION) specialMove = NOSPECIAL;
    numToRowCol(start, end, startRow, endRow, startCol, endCol);
    toPlay = BLACK + WHITE - board[startRow][startCol].getColor();
    color = toPlay;
    movedPiece = board[startRow][startCol];
    captured = board[endRow][endCol];
    board[endRow][endCol] = board[startRow][startCol];
    board[startRow][startCol] = ChessPiece();
    board[endRow][endCol].setMoved(true);

    /*
    if ((board[endRow][endCol].getNameValue() == PAWN) && (qFabs(endRow-startRow) == 2)){
        // update enPassant information
        assignArray(enPassantPos,endRow,endCol);
        enPassantColor = board[endRow][endCol].getColor();
    }
    */

    // handle special moves
    if (specialMove == PROMOTION){
        assignArray(promotionPos, endRow, endCol);
        promote(promoPiece);
    }
    else if (specialMove == ENPASSANT){
        if (board[endRow][endCol].getColor() == BLACK){
            board[endRow-1][endCol] = ChessPiece();
        }
        else{
            board[endRow+1][endCol] = ChessPiece();
        }
    }
    else if (specialMove == BQCASTLE){
        board[0][3] = board[0][0];
        board[0][0] = ChessPiece();
    }
    else if (specialMove == BKCASTLE){
        board[0][5] = board[0][7];
        board[0][7] = ChessPiece();
    }
    else if (specialMove == WQCASTLE){
        board[7][3] = board[7][0];
        board[7][0] = ChessPiece();
    }
    else if (specialMove == WKCASTLE){
        board[7][5] = board[7][7];
        board[7][7] = ChessPiece();
    }

    if (board[endRow][endCol].getNameValue() == KING){
        // if the piece moved is a king update king pos
        if (color == BLACK) assignArray(bkPos, endRow, endCol);
        else assignArray(wkPos, endRow, endCol);
    }

}

bool Board::genMovesForPiece(short int pos, short int moves[]){
    // return the moves that this piece can make
    // return true if it can move

    short int row, col, row1, col1;
    bool canMove = false;
    numToRowCol(pos, 0, row, row1, col, col1);

    switch (board[row][col].getNameValue()) {
        case 1:
            canMove = genPawnMoves(row, col, moves);
            break;
        case 2:
            canMove = genRookMoves(row, col, moves);
            break;
        case 3:
            canMove = genKnightMoves(row, col, moves);
            break;
        case 4:
            canMove = genBishopMoves(row, col, moves);
            break;
        case 5:
            canMove = genQueenMoves(row, col, moves);
            break;
        case 6:
            canMove = genKingMoves(row, col, moves);
            break;
        default:
            moves[0] = -1;
            break;
    }
    return canMove;

}

void Board::moveStats(short int &pieceMoved, short int &color, bool &capture, bool &check, bool &checkmate){
    // return information about move stats

    pieceMoved = movedPiece.getNameValue();
    color = movedPiece.getColor();
    capture = (captured.getNameValue() != EMPTY);
    check = CHECK;
    checkmate = MATE;
}

bool Board::promote(short int pieceNameVal)
{
    // promote a pawn to the given piece type
    // return true if it worked

    if (specialMove == PROMOTION) {

        moveList.setEnd();
        ChessPiece piece;
        bool valid = true;
        short int queenVal = 90, knightVal = 30, rookVal = 50, bishopVal = 30, color = BLACK;

        if (promotionPos[0] == 0) {
            color = WHITE;
            queenVal *= -1;
            knightVal *= -1;
            rookVal *= -1;
            bishopVal *= -1;
        }

        switch (pieceNameVal) {
        case 5:
            piece = ChessPiece(queenVal, QUEEN, color);
            break;
        case 3:
            piece = ChessPiece(knightVal, KNIGHT, color);
            break;
        case 2:
            piece = ChessPiece(rookVal, ROOK, color);
            break;
        case 4:
            piece = ChessPiece(bishopVal, BISHOP, color);
            break;
        default:
            valid = false;
            break;
        }

        if (valid) {
            board[promotionPos[0]][promotionPos[1]] = piece;
            specialMove = NOSPECIAL;
            promoTo = pieceNameVal;
            short int rowList[16], colList[16];
            short int *pos = (color == BLACK) ? wkPos : bkPos;
            if (isChecked(pos[0], pos[1], board, 1-color, rowList, colList, true)){
                isMate(pos[0], pos[1], board, rowList, colList);
            }
            moveList.addPromo(promoTo, CHECK, MATE);
            promoTo = -1;
        }
        return valid;
    }

    return false;
}

void Board::addMove(short int start, short int end){
    // add a move to the movelist

    bool capture = captured.getNameValue() != EMPTY;
    moveList.createMove(start, end, enPassantPos, enPassantColor, specialMove, capture, promoTo, captured, MATE, movedStore, CHECK);
}

bool Board::inBounds(short int row, short int col) {
    return ((row >= 0) && (row < BOARDSIZE) && (col >= 0) && (col < BOARDSIZE));
}

int Board::value()
{
    // sum up the value of the board

    int val = 0;
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            val += board[i][j].getValue();
        }
    }
    return val;
}

void Board::switchToPlay()
{
    toPlay = WHITE + BLACK - toPlay;
}

bool Board::numToRowCol(short int start, short int end, short int &startRow, short int &endRow, short int &startCol, short int &endCol)
{
    // start/end goes from 0-63
    // a1 is bottom left, h8 is the top right

    startRow = start/BOARDSIZE;
    endRow = end/BOARDSIZE;
    startCol = start%BOARDSIZE;
    endCol = end%BOARDSIZE;

    return (inBounds(startRow, startCol) && inBounds(endRow, endCol));
}

short int Board::rowColToNum(short int row, short int col)
{
    // return a number 0-63 for row/start

    return (row*BOARDSIZE + col);
}

short int Board::getSpecial()
{
    return specialMove;
}

bool Board::gameOver()
{
    return (MATE || DRAW);
}

short int Board::getWinner() {
    // returns -1 if the game is not over/draw, or the color of the player who won
    if (MATE) return 1 - toPlay;
    return -1;
}

short int Board::getWinReason(){
    if (MATE) return 0;
    if (STALEMATE) return 1;
    if (INSUFFMAT) return 2;
    return 3;
}

bool Board::isStart(){
    return moveList.isStart();
}

bool Board::isEnd(){
    return moveList.isEnd();
}

bool Board::areEqual(short int arr1[2], short int arr2[2]){
    // check if two arrays are equal
    // equal arrays have same number at each position

    return ((arr1[0] == arr2[0]) && (arr1[1] == arr2[1]));
}

void Board::assignArray(short int arr[2], short int val1, short int val2){
    // put val1/2 in array
    arr[0] = val1;
    arr[1] = val2;
}



