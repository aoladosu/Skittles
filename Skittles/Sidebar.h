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
    QLabel *error;
    QGridLayout *result;
    QGridLayout *settings;
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
    QPushButton *settingsBtn;
    QButtonGroup btnGroup;

    // private functions
    void createSettings();

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();
    QPushButton* getBackButton();
    QPushButton* getForwardButton();
    QButtonGroup* getPromotionBtnGroup();
    QPushButton* getGameOverButton();
    QPushButton* getSettingsButton();
    void addMove(short int piece, short int start, short int end, short int color, bool capture, bool check, bool checkmate, short int special, short int promoPiece, short int value);
    void clearMovelist();
    void removeMove(short int color);
    void showPromotion(QIcon queenIcon, QIcon knightIcon, QIcon bishopIcon, QIcon rookIcon);
    void hidePromotion();
    void showGameOver(short int winner, short int reason);
    void hideGameOver();
    void showError(short int err);
    void hideError();
    void showSettings();
    void hideSettings();

signals:

};

#endif // SIDEBAR_H
