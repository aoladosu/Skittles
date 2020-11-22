#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QButtonGroup>
#include <QGridLayout>
#include <QIcon>
#include <QColor>
#include <QMouseEvent>
#include <QPushButton>
#include "Engine.h"
#include "Sidebar.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    // layout/organization
    QWidget *window;
    QButtonGroup btnGroup;
    QGridLayout *layout;
    Sidebar *sidebar;

    // colors for board
    QColor defaultDarkerColor = QColor(46, 46, 46);
    QColor defaultLighterColor = QColor(237, 237, 237);
    QColor darkerColor = QColor(46, 46, 46);
    QColor lighterColor = QColor(237, 237, 237);
    QColor selectColor = QColor(0, 255, 0);
    QColor pMoveColor = QColor(0, 0, 255);
    QColor cMoveColor = QColor(255, 0, 0);
    QColor aMoveColor = QColor(255, 255, 0);
    QColor checkColor = QColor(255, 165, 0);

    // images
    QIcon blackPawnIcon = QIcon(":/imagesPieces/black_pawn");
    QIcon blackRookIcon = QIcon(":/imagesPieces/black_rook");
    QIcon blackKnightIcon = QIcon(":/imagesPieces/black_knight");
    QIcon blackBishopIcon = QIcon(":/imagesPieces/black_bishop");
    QIcon blackQueenIcon = QIcon(":/imagesPieces/black_queen");
    QIcon blackKingIcon = QIcon(":/imagesPieces/black_king");
    QIcon whitePawnIcon = QIcon(":/imagesPieces/white_pawn");
    QIcon whiteRookIcon = QIcon(":/imagesPieces/white_rook");
    QIcon whiteKnightIcon = QIcon(":/imagesPieces/white_knight");
    QIcon whiteBishopIcon = QIcon(":/imagesPieces/white_bishop");
    QIcon whiteQueenIcon = QIcon(":/imagesPieces/white_queen");
    QIcon whiteKingIcon = QIcon(":/imagesPieces/white_king");

    // what pieces to move
    short int firstClick = -1;
    short int secondClick = -1;

    // what squares to highlight
    short int pMoves[35];               // where piece can move
    short int cMoves[10];               // where they can move and capture
    short int aMoves[10];               // pieces that are attacking this one
    short int chMoves[6] = {-1};        // what pieces are attacking king

    // chess engine
    Engine engine;
    bool engineTurn = false;
    bool enginePlay = true;

    // board variables
    const short int BOARDSIZE = 8;
    const short int WHITE = 0;
    const short int BLACK = 1;

    // promotion variables
    short int promotionColor;
    short int promotionPos;
    bool promotionOpen;
    short int promotionStart;

    // undo redo variables
    short int oldSpecial= -1;

    // private functions
    void buttonStartup();
    void setImages();
    void createButton( QSizePolicy sizePolicy, short int id, short int row, short int col);
    void restoreButtonColor(QPushButton *btn, short int id);
    void handleMove(QPushButton *btn1, QPushButton *btn2, short int special, short int start, short int end, short int color);
    void handleUndoMove(short int start, short int end, short int special, short int capturedPiece, short int color);
    void enPassant(short int clickOne, short int clickTwo);
    void castle(short int castleType);
    void addLabels();
    void insertLabels(QSizePolicy sizePolicy, QString text, short int row, short int col);
    void highlightButtons(short int btnList[], QColor color, bool highlight);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *object, QEvent *event) override;

private slots:
    void buttonPressed(int id);
    void undoMove();
    void redoMove();
    void promotion(int id);
    void newGame();
};

