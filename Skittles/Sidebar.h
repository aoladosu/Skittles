#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QButtonGroup>
#include <QIcon>

class Sidebar : public QWidget
{
    Q_OBJECT

private:

    // variables for layouts in sidebar
    QGridLayout *gridLayout;
    QLabel *title;
    QGridLayout *result;
    QListWidget *moveList;
    QHBoxLayout *bottom;
    QPushButton *back;
    QPushButton *forward;
    short int moveNum = 0;

    // buttons for promotion
    QPushButton *queen;
    QPushButton *knight;
    QPushButton *bishop;
    QPushButton *rook;
    QPushButton *gameOverBtn;
    QButtonGroup btnGroup;

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();
    QPushButton* getBackButton();
    QPushButton* getForwardButton();
    QButtonGroup* getPromotionBtnGroup();
    QPushButton* getGameOverButton();
    void addMove(short int piece, short int start, short int end, short int color, bool capture, bool check, bool checkmate, short int special, short int promoPiece);
    void clearMovelist();
    void removeMove(short int color);
    void showPromotion(QIcon queenIcon, QIcon knightIcon, QIcon bishopIcon, QIcon rookIcon);
    void hidePromotion();
    void showGameOver(short int winner, short int reason);
    void hideGameOver();


signals:

};

#endif // SIDEBAR_H
