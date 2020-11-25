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
#include <QCheckBox>
#include <QObject>
#include <QEvent>

class Sidebar : public QWidget
{
    Q_OBJECT

private:

    // variables for layouts in sidebar
    QGridLayout *gridLayout;
    QPushButton *title;
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
    QButtonGroup promoBtnGroup;

    // settings and game over buttons
    QPushButton *gameOverBtn;
    QPushButton *settingsBtn;

    // settings buttons and button groups
    QButtonGroup depthBtnGroup;
    QButtonGroup policyBtnGroup;
    QButtonGroup colorBtnGroup;
    QCheckBox *highlight, *agent;
    QWidget *settingsContainer;
    QWidget *instructContainer;
    QLabel *instructions;

    // whether settings is open or not
    bool settingsOpen = false;
    bool titleOpen = false;

    // private functions
    void createSettings();
    void createInstructions();
    void showSettings();
    void hideSettings();
    void showMoveList();
    void hideMoveList();
    void showTitle();
    void hideTitle();

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();
    QPushButton* getBackButton();
    QPushButton* getForwardButton();
    QPushButton* getGameOverButton();
    QCheckBox* getHighlightButton();
    QCheckBox* getAgentButton();
    QButtonGroup* getPromotionBtnGroup();
    QButtonGroup* getDepthBtnGroup();
    QButtonGroup* getPolicyBtnGroup();
    QButtonGroup* getColorBtnGroup();
    void addMove(short int piece, short int start, short int end, short int color, bool capture, bool check, bool checkmate, short int special, short int promoPiece, short int value);
    void clearMovelist();
    void removeMove(short int color);
    void showPromotion(QIcon queenIcon, QIcon knightIcon, QIcon bishopIcon, QIcon rookIcon);
    void hidePromotion();
    void showGameOver(short int winner, short int reason);
    void hideGameOver();
    void showError(short int err);
    void hideError();
    void setTitleSize();

signals:

private slots:
    void settingsClicked();
    void titleClicked();

};

#endif // SIDEBAR_H
