#include "Board.h"
#include <QtMath>

Board::Board(){}
Board::~Board(){}

// validate - add these functions
// strToNum - add these functions

void Board::init()
{
    // initialize the board with a new game and place pieces on board

    int blackVals[] = {50, 30, 30, 90, 900, 30, 30, 50};
    int blackpawnVals = 10;
    int blackNameVals[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
    int whiteVals[] = {-50, -30, -30, -90, -900, -30, -30, -50};
    int whitepawnVals = -10;
    int whiteNameVals[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };

    for (int i = 0; i < BOARDSIZE; i++) {
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

bool Board::validate(int start, int end)
{
    // check whether a move is valid and move piece if it is

    bool valid = false;

    if (MATE) return false;							// the game has been won
    if (specialMove == PROMOTION) return false;		// a pawn wasn't promoted, that needs to be done first
    specialMove = NOSPECIAL;						// reset to no special, it should be checked by front end after every turn ...

    int startRow, endRow, startCol, endCol;
    if (!numToRowCol(start, end, startRow, endRow, startCol, endCol)) {
        return false;
    }

    ChessPiece piece = board[startRow][startCol], startPiece, endPiece, behindPiece;		// keep track of board state
    bool moved;																				// store if piece was moved
    int rowList[16], colList[16], *currKing, *othKing, oppColor = WHITE + BLACK - toPlay;	// to check checkmate
    int kingRow, kingCol;																	// king's position

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

        // store past state of board
        startPiece = board[startRow][startCol];
        moved = startPiece.getMoved();
        endPiece = board[endRow][endCol];
        if (startPiece.getNameValue() == PAWN) {
            if (startPiece.getColor() == BLACK) behindPiece = board[endRow - 1][endCol];
            else behindPiece = board[endRow + 1][endCol];
        }

        // move piece
        movePiece(startRow, endRow, startCol, endCol);

        // get king's position, might have changed if the king was moved
        if (piece.getNameValue() == KING) {
            kingRow = endRow;
            kingCol = endCol;
        }
        else {
            kingRow = currKing[0];
            kingCol = currKing[1];
        }

        // check if move causes this king to be in check
        if (isChecked(kingRow, kingCol, board, toPlay, rowList, colList)) {

            // undo move
            if (startPiece.getNameValue() == PAWN) {
                if (startPiece.getColor() == BLACK) board[endRow - 1][endCol] = behindPiece;
                else board[endRow + 1][endCol] = behindPiece;
            }
            board[startRow][startCol] = startPiece;
            board[startRow][startCol].setMoved(moved);
            board[endRow][endCol] = endPiece;
            specialMove = NOSPECIAL;
            assignArray(enPassantPos, enPassantPosOld[0], enPassantPosOld[0]);
            enPassantColor = enPassantColorOld;

            return false;
        }

        // check if opposite king is in checkmate
        if (isChecked(othKing[0], othKing[1], board, oppColor, rowList, colList)) {
            if (isMate(othKing[0], othKing[1], board, rowList, colList)) {
                MATE = true;
            }
        }

        // update kings' positions
        if (piece.getNameValue() == KING) {
            currKing[0] = endRow;
            currKing[1] = endCol;
        }
        switchToPlay();
        int temp[2] = { -2,-2 };
        if (!areEqual(enPassantPos,temp) && (enPassantColor != piece.getColor())) {
            assignArray(enPassantPos,temp[0],temp[1]);
        }
        // store passant values
        assignArray(enPassantPosOld, enPassantPos[0], enPassantPos[1]);
        enPassantColorOld = enPassantColor;

        // add move to movelist
        //addMove(start, end);
    }

    return valid;
}

bool Board::pawnMoveValid(ChessPiece pawn, int startRow, int endRow, int startCol, int endCol)
{
    // validate pawn movement

    bool valid = false;

    int start[2] = {startRow, startCol};
    int end[2] = { endRow, endCol };
    int color = pawn.getColor();
    int row;
    int pos1[2];
    int pos2[2];
    int pos3[2];
    int pos4[2];

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

bool Board::rookMoveValid(ChessPiece rook, int startRow, int endRow, int startCol, int endCol)
{
    // validate rook movements
    // castling is done in king verification

    bool valid = false;
    int rowShift = (int)(startRow != endRow);
    int colShift = (int)(startCol != endCol);

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

    for (int row = startRow + rowShift, col = startCol + colShift; ((row != endRow) || (col != endCol)); row += rowShift, col += colShift) {
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

bool Board::knightMoveValid(ChessPiece knight, int startRow, int endRow, int startCol, int endCol)
{
    // validate knight movements

    bool valid = false;

    int pos1[] = { startRow - 2, startCol + 1 };	// 2up 1right
    int pos2[] = { startRow - 1, startCol + 2 };	// 1up 2right
    int pos3[] = { startRow + 1, startCol + 2 };	// 1down 2right
    int pos4[] = { startRow + 2, startCol + 1 };	// 2down 1right
    int pos5[] = { startRow + 2, startCol - 1 };	// 2down 1left
    int pos6[] = { startRow + 1, startCol - 2 };	// 1down 2left
    int pos7[] = { startRow - 1, startCol - 2 };	// 1up 2left
    int pos8[] = { startRow - 2, startCol - 1 };	// 2up 1left
    int end[] = { endRow, endCol };

    if (knight.getColor() == board[endRow][endCol].getColor()) {
        // can't capture own piece
        valid = false;
    }
    else if (areEqual(end,pos1) || areEqual(end,pos2) || areEqual(end,pos3) || areEqual(end,pos4) || areEqual(end,pos5) || areEqual(end,pos6) || areEqual(end,pos7) || areEqual(end,pos8)){
        valid = true;
    }
    return valid;
}

bool Board::bishopMoveValid(ChessPiece bishop, int startRow, int endRow, int startCol, int endCol)
{
    // validate bishop moves

    bool valid = false;
    int rowShift = 1;
    int colShift = 1;

    if (qFabs(endRow - startRow) != qFabs(endCol - startCol)) { // TODO: make sure qFabs working as intended
        // didn't move diagonally
        return valid;
    }

    if (endCol < startCol) {
        colShift = -1;
    }

    if (endRow < startRow) {
        rowShift = -1;
    }

    for (int row = startRow + rowShift, col = startCol + colShift; ((row != endRow) || (col != endCol)); row += rowShift, col += colShift) {
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

bool Board::queenMoveValid(ChessPiece queen, int startRow, int endRow, int startCol, int endCol)
{
    // validate queen movement
    // queen movement is a combination of bishop and rook movement

    return ((bishopMoveValid(queen, startRow, endRow, startCol, endCol)) || (rookMoveValid(queen, startRow, endRow, startCol, endCol)));
}

bool Board::kingMoveValid(ChessPiece king, int startRow, int endRow, int startCol, int endCol)
{
    // validate king movements
    // castling rules: king/rook can't have moved, nothing between king/rook, king not in check and
    // does not pass/stop on square attacked by enemy

    bool valid = false;
    int rList[16], cList[16];

    int pos1[2] = { startRow - 1, startCol + 0 };	// 1up 0right
    int pos2[2] = { startRow - 1, startCol + 1 };	// 1up 1right
    int pos3[2] = { startRow + 0, startCol + 1 };	// 0down 1right
    int pos4[2] = { startRow + 1, startCol + 1 };	// 1down 1right
    int pos5[2] = { startRow + 1, startCol - 0 };	// 1down 0left
    int pos6[2] = { startRow + 1, startCol - 1 };	// 1down 1left
    int pos7[2] = { startRow - 0, startCol - 1 };	// 0up 1left
    int pos8[2] = { startRow - 1, startCol - 1 };	// 1up 1left
    int pos9[2] = { startRow, startCol + 2 };	    // king side castle
    int pos10[2] = { startRow, startCol - 2 };	    // queen side castle
    int end[2] = { endRow, endCol };

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

void Board::movePiece(int startRow, int endRow, int startCol, int endCol)
{
    // move the chess piece to the new location

    ChessPiece piece = board[startRow][startCol];
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

bool Board::isChecked(int row, int col, ChessPiece cboard[8][8], int color, int rowList[], int colList[]) {

    // check if the given row/col is attacked by a piece
    // color is the the perspective to look from in case row/col refers to an empty square
    // that is color is color of the piece that is (or would be) occupying the square


    if (!inBounds(row, col)) return false;

    ChessPiece piece;
    int index = 0;

    // pawn positions
    int prowPos;
    if (color == BLACK) {
        prowPos = row + 1;
    }
    else {
        prowPos = row - 1;
    }
    int pcolPos[2] = { col + 1, col - 1 };

    // check if pawn is attacking row,col
    for (int i = 0; i < 2; i++) {
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
    int krowPos[8] = { row - 2, row - 1, row + 1, row + 2, row + 2, row + 1, row - 1, row - 2 };
    int kcolPos[8] = { col + 1, col + 2, col + 2, col + 1, col - 1, col - 2, col - 2, col - 1 };
    // check if knight is attacking row,col
    for (int i=0; i<BOARDSIZE; i++){
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
    for (int i = row - 1; i < row + 2; i++) {
        for (int j = col - 1; j < col + 2; j++) {
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
    for (int i = row - 1, j = col; i >= 0; i--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or rook downward
    for (int i = row + 1, j = col; i < BOARDSIZE; i++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or rook leftward
    for (int i = row, j = col-1; j >=0; j--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or rook rightward
    for (int i = row, j = col + 1; j < BOARDSIZE; j++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == ROOK)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop up-rightward
    for (int i = row - 1, j = col + 1; ((i >= 0) && (j < BOARDSIZE)); i--, j++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop up-leftward
    for (int i = row - 1, j = col - 1; ((i >= 0) && (j >= 0)); i--, j--) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop down-rightward
    for (int i = row + 1, j = col + 1; ((i < BOARDSIZE) && (j < BOARDSIZE)); i++, j++) {
        piece = cboard[i][j];
        if (((piece.getNameValue() == QUEEN) || (piece.getNameValue() == BISHOP)) && (piece.getColor() != color)) {
            rowList[index] = i;
            colList[index] = j;
            index++;
        }
        if (piece.getNameValue() != EMPTY) break;
    }

    // check if there is a queen or bishop down-leftward
    for (int i = row + 1, j = col - 1; ((i < BOARDSIZE) && (j >= 0)); i++, j--) {
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

    return (index != 0);
}

bool Board::isMate(int row, int col, ChessPiece cboard[8][8], int rowList[], int colList[]) {
    // check if the king at row/col is in check mate, assumes the king being in check has been checked

    if (cboard[row][col].getNameValue() != KING) return false;
    int rowInfo[16], colInfo[16];
    int color = cboard[row][col].getColor();
    int oppColor = WHITE + BLACK - color;
    int i = -1;
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
    for (int j = row - 1; j < row + 2; j++) {
        for (int k = col - 1; k < col + 2; k++) {
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

    if ((i == 2) || (cboard[rowList[0]][colList[0]].getNameValue() == KNIGHT)) {
        // with a 2piece check if the king can't move, then it is check mate, or a knight can't be los'd
        return true;
    }

    // else see if a piece can block los
    int startRow = row;
    int startCol = col;
    int endRow = rowList[0];
    int endCol = colList[0];
    int rowShift, colShift;

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

    for (int rowIndex = startRow + rowShift, colIndex = startCol + colShift; ((rowIndex != endRow) || (colIndex != endCol)); rowIndex += rowShift, colIndex += colShift) {
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

bool Board::isPawnLos(int row, int col, ChessPiece cboard[8][8], int color){
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

void Board::goBack(int &start, int &end, int &special, int &promoPiece, int &capturedPiece)
{
    // go forward a move
    // TODO: finish changing board, how to undo a piece bing moved beofre

    Move *move = moveList.getPrevious();
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
    ChessPiece captPiece = ChessPiece(move->piece);
    capturedPiece = captPiece.getNameValue();

    // set board variables back
    assignArray(enPassantPos, move->enPassant[0], move->enPassant[1]);
    enPassantColor = move->enPassantColor;
    specialMove = NOSPECIAL;
    MATE = move->mate;

    // set pieces including captured back
    int startRow, endRow, startCol, endCol, specialmv;
    specialmv = move->specialMove;
    numToRowCol(start, end, startRow, endRow, startCol, endCol);
    board[startRow][startCol] = board[endRow][endCol];
    if (specialmv == ENPASSANT){
        if (board[startRow][startCol].getColor() == BLACK){
            board[endRow-1][endCol] = captPiece;
        }
        else{
            board[endRow+1][endCol] = captPiece;
        }
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
        board[0][3] = ChessPiece();
    }
    else if (specialmv == BKCASTLE){
        board[0][7] = board[0][5];
        board[0][5] = ChessPiece();
    }
    else if (specialmv == WQCASTLE){
        board[7][0] = board[7][3];
        board[7][3] = ChessPiece();
    }
    else if (specialmv == WKCASTLE){
        board[7][7] = board[7][5];
        board[7][5] = ChessPiece();
    }


    toPlay = board[startRow][startCol].getColor();
}

void Board::goForward(int &start, int &end, int &special, int &promoPiece, int &capturedPiece){
    // go foraward a move

    // TODO: finish changing board

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
    //assignArray(enPassantPos, move->enPassant[0], move->enPassant[1]);
    //enPassantColor = move->enPassantColor;
    MATE = move->mate;
    int startRow, endRow, startCol, endCol;
    specialMove = move->specialMove;
    numToRowCol(start, end, startRow, endRow, startCol, endCol);
    toPlay = BLACK + WHITE - board[startRow][startCol].getColor();
    board[endRow][endCol] = board[startRow][startCol];
    board[endRow][endCol].setMoved(true);

    if ((board[endRow][endCol].getNameValue() == PAWN) && (qFabs(endRow-startRow) == 2)){
        // update enPassant information
        assignArray(enPassantPos,endRow,endCol);
    }

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

}

bool Board::promote(int pieceNameVal)
{
    // promote a pawn to the given piece type
    // return true if it worked

    if (specialMove == PROMOTION) {

        ChessPiece piece;
        bool valid = true;
        int queenVal = 90, knightVal = 30, rookVal = 50, bishopVal = 30, color = BLACK;

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
        }

        return valid;
    }

    return false;
}

void Board::addMove(int start, int end){
    // add a move to the movelist

    bool capture = captured.getNameValue() != EMPTY;
    moveList.createMove(start, end, enPassantPosOld, enPassantColorOld, specialMove, capture, promoTo, captured, MATE);
}

bool Board::inBounds(int row, int col) {
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

bool Board::numToRowCol(int start, int end, int &startRow, int &endRow, int &startCol, int &endCol)
{
    // start/end goes from 0-63
    // a1 is bottom left, h8 is the top right

    startRow = start/BOARDSIZE;
    endRow = end/BOARDSIZE;
    startCol = start%BOARDSIZE;
    endCol = end%BOARDSIZE;

    return (inBounds(startRow, startCol) && inBounds(endRow, endCol));
}

int Board::rowColToNum(int row, int col)
{
    // return a number 0-63 for row/start

    return (row*BOARDSIZE + col);
}

int Board::getSpecial()
{
    return specialMove;
}

bool Board::gameOver()
{
    return MATE;
}

int Board::getWinner() {
    // returns -1 if the game is not over, or the color of the player who won
    if (MATE) {
        return toPlay;
    }
    return -1;
}

bool Board::areEqual(int arr1[2], int arr2[2]){
    // check if two arrays are equal
    // equal arrays have same number at each position

    return ((arr1[0] == arr2[0]) && (arr1[1] == arr2[1]));
}

void Board::assignArray(int arr[2], int val1, int val2){
    // put val1/2 in array
    arr[0] = val1;
    arr[1] = val2;
}



