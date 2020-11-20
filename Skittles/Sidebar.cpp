#include <QLayout>
#include <QListView>
#include <QSizePolicy>
#include <QString>
#include "Sidebar.h"


Sidebar::Sidebar(QWidget *parent) : QWidget(parent)
{

    // create layout
    gridLayout = new QGridLayout(this);

    // size policy for buttons
    QSizePolicy sPolicy = QSizePolicy();
    sPolicy.setHorizontalPolicy(QSizePolicy::Minimum);
    sPolicy.setVerticalPolicy(QSizePolicy::Minimum);

    // create promotion buttons
    queen = new QPushButton("Queen", this);
    queen->setSizePolicy(sPolicy);
    queen->hide();
    knight = new QPushButton("Knight", this);
    knight->setSizePolicy(sPolicy);
    knight->hide();
    bishop = new QPushButton("Bishop", this);
    bishop->setSizePolicy(sPolicy);
    bishop->hide();
    rook = new QPushButton("Rook", this);
    rook->setSizePolicy(sPolicy);
    rook->hide();
    btnGroup.addButton(queen, 5);
    btnGroup.addButton(knight, 3);
    btnGroup.addButton(bishop, 4);
    btnGroup.addButton(rook, 2);

    // create game over button
    gameOverBtn = new QPushButton(this);
    gameOverBtn->setSizePolicy(sPolicy);

    // create layout elements
    title = new QLabel(QString("Skittles"), this);

    error = new QLabel(this);
    error->setSizePolicy(sPolicy);
   error->setWordWrap(true);

    moveList = new QListWidget(this);
    moveList->setResizeMode(QListView::Adjust);
    moveList->setViewMode(QListView::ListMode);
    moveList->setUniformItemSizes(true);
    moveList->setAlternatingRowColors(true);
    moveList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    moveList->setSizePolicy(sPolicy);

    result = new QGridLayout(this);
    result->addWidget(gameOverBtn);

    bottom = new QHBoxLayout(this);
    QIcon backIcon = QIcon(":/buttonImages/back_arrow");
    QIcon forwardIcon = QIcon(":/buttonImages/forward_arrow");
    back = new QPushButton(backIcon, "", this);
    forward = new QPushButton(forwardIcon, "", this);
    QBoxLayout *bottom2 = (QBoxLayout *) bottom;
    bottom2->addWidget(back);
    bottom2->addWidget(forward);

    // add elements to layout
    gridLayout->addWidget(title,0,0,1,1);
    gridLayout->addWidget(moveList,1,0,5,0);
    gridLayout->addLayout((QLayout *) result, 6, 0, 1, 1);
    gridLayout->addLayout((QLayout *) bottom, 7, 0, 1, 1);

    // set layout
    QLayout *gridLayout2 = (QLayout *) gridLayout;
    gridLayout2->setContentsMargins(10,10,10,10);
    this->setLayout((QLayout *) gridLayout);
    this->show();
    hideGameOver();

}

void Sidebar::addMove(short int piece, short int start, short int end, short int color, bool capture, bool check, bool checkmate, short int special, short int promoPiece){
    // add move to move list

    QString string = "";
    short int row = end/8;
    short int col = end%8;

    // which piece moved
    switch (piece){
        case 1:
            // pawn
            break;
        case 2:
            // rook
            string += "R";
            break;
        case 3:
            // knight
            string += "N";
            break;
        case 4:
            // bishop
            string += "B";
            break;
        case 5:
            // queen
            string += "Q";
            break;
        case 6:
            // king
            string += "K";
            break;
        default:
            break;
    }

    // add capture notation
    if (capture) string += "x";

    // to what col
    switch (col){
        case 0:
            string += "a";
            break;
        case 1:
            string += "b";
            break;
        case 2:
            string += "c";
            break;
        case 3:
            string += "d";
            break;
        case 4:
            string += "e";
            break;
        case 5:
            string += "f";
            break;
        case 6:
            string += "g";
            break;
        case 7:
            string += "h";
            break;
        default:
            break;
    }

    // to what row
    switch (row){
        case 0:
            string += "8";
            break;
        case 1:
            string += "7";
            break;
        case 2:
            string += "6";
            break;
        case 3:
            string += "5";
            break;
        case 4:
            string += "4";
            break;
        case 5:
            string += "3";
            break;
        case 6:
            string += "2";
            break;
        case 7:
            string += "1";
            break;
        default:
            break;
    }

    // add promotion
    switch (promoPiece){
        case -1:
            break;
        case 2:
            // rook
            string += "=R";
            break;
        case 3:
            // knight
            string += "=N";
            break;
        case 4:
            // bishop
            string += "=B";
            break;
        case 5:
            // queen
            string += "=Q";
            break;
        default:
            break;

    }

    if (checkmate) string += "#";
    else if (check) string += "+";

    // special moves
    switch (special) {
        case 3:
            // queen side castle
            string = "0-0-0";
            break;
        case 4:
            // queen side castle
            string = "0-0-0";
            break;
        case 5:
            // king side castle
            string = "0-0";
            break;
        case 6:
            // king side castle
            string = "0-0";
            break;
        default:
            break;
    }

    if (color == 1){
        // black
        string = QString::number(moveNum) + ".  Black     " +  string;
    }
    else{
        moveNum += 1;
        string = QString::number(moveNum) + ".  White    " +  string;
    }

    moveList->addItem(string);
    moveList->scrollToBottom();
}

void Sidebar::removeMove(short int color){

    if (color == 0){
        // white
        moveNum -= 1;
    }

    moveList->takeItem(moveList->count()-1);

}

void Sidebar::clearMovelist(){
    // clear movelist

    moveList->clear();
    moveNum = 0;

}

void Sidebar::showPromotion(QIcon queenIcon, QIcon knightIcon, QIcon bishopIcon, QIcon rookIcon){
    // add promotion options to sidebar

    queen->setIcon(queenIcon);
    knight->setIcon(knightIcon);
    bishop->setIcon(bishopIcon);
    rook->setIcon(rookIcon);

    result->addWidget(queen, 0, 0);
    result->addWidget(knight, 0, 1);
    result->addWidget(bishop, 1, 0);
    result->addWidget(rook, 1, 1);

    queen->show();
    knight->show();
    bishop->show();
    rook->show();
}

void Sidebar::hidePromotion(){
    // remove promotion options from sidebar

    QLayout *result2 = (QLayout *) result;

    result2->removeWidget(queen);
    result2->removeWidget(knight);
    result2->removeWidget(bishop);
    result2->removeWidget(rook);

    queen->hide();
    knight->hide();
    bishop->hide();
    rook->hide();
}

void Sidebar::showError(short int err){

    /*
        0-mate,                         // one side has won from check, game can't continue
        1-draw,                         // game has been drawn, can't continue
        2-promotion,                    // a pawn needs to be promoted
        3-boundary,                     // the start or end position was out of bounds
        4-color,                        // the wrong color is trying to play
        5-kingInCheck,                  // the move is valid, but it leaves the king in check
        6-pawnOntoPiece,                // pawn is trying to move onto a piece, but not diagonally for a capture
        7-pawnDoubleSpace,              // pawn is trying to move forward by two, but it has already moved
        8-selfCapture,                  // trying to capture own piece
        9-enpassant,                    // trying to enpassant, but conditions not met
        10-jump,                         // a piece is trying to jump over another
        11-castle,                       // one or more of the castling rules was not met
        12-badMovement,                  // piece not moved correctly
        13-none,                          // no errors, everything handles well
     */


    //show error
    switch (err){
        case 0:
            error->setText("Error:\nGame is over from checkmate");
            break;
        case 1:
            error->setText("Error:\nGame is over from draw");
        case 2:
            error->setText("Error:\nPAWN needs to be promoted first");
            break;
        case 3:
            error->setText("Error:\nThis move goes out of bounds");
            break;
        case 4:
            error->setText("Error:\nIt is the other color's turn to play");
            break;
        case 5:
            error->setText("Error:\nThis move leaves the KING in check");
            break;
        case 6:
            error->setText("Error:\nA PAWN cannot not move onto another piece unless it is capturing diagonally");
            break;
        case 7:
            error->setText("Error:\nThis PAWN has already been moved and therefore cannot be moved by two spaces anymore");
            break;
        case 8:
            error->setText("Error:\nCannot capture own piece");
            break;
        case 9:
            error->setText("Error:\nConditions for en passant have not been met.\nTo meet en passant conditions, an opponent's PAWN, must have just moved by two when you try to capture behind it");
            break;
        case 10:
            error->setText("Error:\nThis piece cannot jump over other pieces");
            break;
        case 11:
            error->setText("Error:\nConditions for castling are not met\n1. KING and ROOK cannot have moved\n2. KING cannot be in check\n3. No pieces are between the KING and ROOK\n4. Each square the KING passes cannot be under attack");
            break;
        case 12:
            error->setText("Error:\nThis piece does not have this movement");
            break;
        default:
            error->setText("No error");
            break;
    }

    error->show();
    result->addWidget(error, 0, 0);

}

void Sidebar::hideError(){

    QLayout *result2 = (QLayout *) result;
    result2->removeWidget(error);
    error->hide();
}

void Sidebar::showGameOver(short int winner, short int reason){
    // show game over
    if (winner == 1){
        gameOverBtn->setText("Black Wins!\n\n0 - 1\n(Click for new game)");
    }
    else if (winner == 0){
        gameOverBtn->setText("White Wins!\n\n1 - 0\n(Click for new game)");
    }
    else if (reason == 1){
        gameOverBtn->setText("Draw - Stalemate!\n\n1/2 - 1/2\n(Click for new game)");
    }
    else{
        gameOverBtn->setText("Draw - Insufficient Material!\n\n1/2 - 1/2\n(Click for new game)");
    }
    gameOverBtn->show();
    result->addWidget(gameOverBtn, 0, 0);
}

void Sidebar::hideGameOver(){
    // hide game overr

    QLayout *result2 = (QLayout *) result;
    result2->removeWidget(gameOverBtn);
    gameOverBtn->hide();
}

QPushButton* Sidebar::getBackButton(){
    return back;
}

QPushButton* Sidebar::getForwardButton(){
    return forward;
}

QButtonGroup* Sidebar::getPromotionBtnGroup(){
    return &btnGroup;
}

QPushButton* Sidebar::getGameOverButton(){
    return gameOverBtn;
}

Sidebar::~Sidebar(){
    delete title;
    delete gameOverBtn;
    delete bottom;
    delete back;
    delete forward;
    delete result;
    delete gridLayout;
}
