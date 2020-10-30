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
    QColor altSelectColor = QColor(255, 0, 0);

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
    int firstClick = -1;
    int secondClick = -1;

    // chess engine
    Engine engine;

    // board variables
    const int BOARDSIZE = 8;
    const int WHITE = 0;
    const int BLACK = 1;

    // private functions
    void buttonStartup();
    void setImages();
    void createButton( QSizePolicy sizePolicy, short int id, int row, int col);
    void restoreButtonColor(QPushButton *btn, int id);
    void handleMove(QPushButton *btn1, QPushButton *btn2, int special, int start, int end);
    void handleUndoMove(int start, int end, int special, int capturedPiece, int color);
    void enPassant(int clickOne, int clickTwo);
    void castle(int castleType);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *object, QEvent *event) override;

private slots:
    void buttonPressed(int id);
    void undoMove();
    void redoMove();


};

