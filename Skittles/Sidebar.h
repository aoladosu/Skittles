#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

class Sidebar : public QWidget
{
    Q_OBJECT

private:

    // variables for layouts in sidebar
    QGridLayout *gridLayout;
    QLabel *title;
    QHBoxLayout *result;
    QListWidget *moveList;
    QHBoxLayout *bottom;
    QPushButton *back;
    QPushButton *forward;


    // images
    QIcon backIcon = QIcon(":/buttonImages/back_arrow");
    QIcon forwardIcon = QIcon(":/buttonImages/forward_arrow");

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();
    QPushButton* getBackButton();
    QPushButton* getForwardButton();
    void addMove(short int piece, short int start, short int end, short int color, bool capture, bool check, bool checkmate, short int special, short int promoPiece);



signals:

};

#endif // SIDEBAR_H