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
    knight = new QPushButton("Knight", this);
    knight->setSizePolicy(sPolicy);
    bishop = new QPushButton("Bishop", this);
    bishop->setSizePolicy(sPolicy);
    rook = new QPushButton("Rook", this);
    rook->setSizePolicy(sPolicy);
    btnGroup.addButton(queen, 5);
    btnGroup.addButton(knight, 3);
    btnGroup.addButton(bishop, 4);
    btnGroup.addButton(rook, 2);

    // create game over button
    gameOverBtn = new QPushButton(this);
    gameOverBtn->setSizePolicy(sPolicy);

    // create layout elements
    title = new QLabel(QString("Skittles"), this);

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
}

void Sidebar::hidePromotion(){
    // remove promotion options from sidebar

    QLayout *result2 = (QLayout *) result;

    result2->removeWidget(queen);
    result2->removeWidget(knight);
    result2->removeWidget(bishop);
    result2->removeWidget(rook);
}

void Sidebar::showGameOver(short int winner, short int reason){

    gameOverBtn->show();
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

    result->addWidget(gameOverBtn, 0, 0);
}

void Sidebar::hideGameOver(){

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
