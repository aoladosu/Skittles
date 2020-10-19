#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QButtonGroup>
#include <QGridLayout>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *window;
    QButtonGroup btnGroup;
    QGridLayout *layout;

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


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setImages();
    bool eventFilter(QObject *object, QEvent *event);
    void createButton( QSizePolicy sizePolicy, short int id, int row, int col, QIcon icon=QIcon());


};

