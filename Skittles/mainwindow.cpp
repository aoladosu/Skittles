#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <Qt>
#include <QSizePolicy>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    //ui->setupUi(this);ui(new Ui::MainWindow)

    window = new QWidget();
    layout = new QGridLayout(this);

    buttonStartup();
    setImages();

    sidebar = new Sidebar(this);
    layout->addWidget(sidebar, 0, 8, 8, 2);

    connect(&btnGroup, SIGNAL(idClicked(int)), this, SLOT(buttonPressed(int)));
    connect(sidebar->getBackButton(), SIGNAL(clicked()), this, SLOT(undoMove()));
    connect(sidebar->getForwardButton(), SIGNAL(clicked()), this, SLOT(redoMove()));
    engine.init();

}

void MainWindow::buttonPressed(int id){
    // handle a button being clicked

    QPushButton *btn2 = (QPushButton *) btnGroup.button(id);


    if (firstClick == -1){
        // set first click and change button to select color
        firstClick = id;
        QPalette pal = btn2->palette();
        pal.setColor(QPalette::Button, selectColor);
        btn2->setPalette(pal);
        return;
    }

    if (id == firstClick){
        // unselect button, clicked twice
        firstClick = -1;
        secondClick = -1;
        restoreButtonColor(btn2, id);
        return;
    }

    // validate move with engine
    secondClick = id;
    QPushButton *btn1 = (QPushButton *) btnGroup.button(firstClick);
    bool valid = engine.validate(firstClick, secondClick);
    restoreButtonColor(btn1, firstClick);

    if (valid){
        // move was valid in chess engine, so move pieces
        handleMove(btn1, btn2, engine.getSpecialMove(), firstClick, secondClick);
    }

    firstClick = -1;
    secondClick = -1;

}

void MainWindow::undoMove(){
    // process undoing a move

    // variables to get info back
    int start, end, special, promoPiece, capturedPiece, color;
    engine.goBack(start, end, special, promoPiece, capturedPiece, color);

    qDebug() << "undo stats...";
    qDebug() << "start: " << start;
    qDebug() << "end: " << end;
    qDebug() << "special: " << special;
    qDebug() << "promo: " << promoPiece;
    qDebug() << "captured: " << capturedPiece;
    qDebug() << "color: " << color;

    if (start == -1){
        // TODO: have a message that you can't go back, or grey out button, so need a new func
        return;
    }

    handleUndoMove(start, end, special, capturedPiece, color);

}

void MainWindow::redoMove(){
    // process redoing a move

    // variables to get info back
    int start, end, special, promoPiece, capturedPiece, color;
    engine.goForward(start, end, special, promoPiece, capturedPiece, color);

    qDebug() << "redo stats...";
    qDebug() << "start: " << start;
    qDebug() << "end: " << end;
    qDebug() << "special: " << special;
    qDebug() << "promo: " << promoPiece;
    qDebug() << "captured: " << capturedPiece;
    qDebug() << "color: " << color;

    if (start == -1){
        // TODO: have a message that you can't go forward, or grey out button, so need a new func
        return;
    }

    QPushButton *btnStart = (QPushButton *) btnGroup.button(start);
    QPushButton *btnEnd = (QPushButton *) btnGroup.button(end);
    handleMove(btnStart, btnEnd, special, start, end);

}

void MainWindow::handleMove(QPushButton *btn1, QPushButton *btn2, int special, int start, int end){
    // move pieces and handle special cases

    btn2->setIcon(btn1->icon());
    btn1->setIcon(QIcon());

    switch (special) {
        case 0:
            // no special
            break;
        case 1:
            // promotion
            // TODO: ask for promotion Promotion();
            break;
        case 2:
            // enpassant
            enPassant(start, end);
            break;
        case 3:
            // bqcastle
            castle(3);
            break;
        case 4:
            // wqcastle
            castle(4);
            break;
        case 5:
            // bkcastle
            castle(5);
            break;
        case 6:
            // wkcastle
            castle(6);
            break;
        default:
            break;
    }

}

void MainWindow::handleUndoMove(int start, int end, int special, int capturedPiece, int color){
    // handle moving pieces back one move
    // color is the piece that moved on this turn

    // get buttons
    QPushButton *btnStart = (QPushButton *) btnGroup.button(start);
    QPushButton *btnEnd = (QPushButton *) btnGroup.button(end);
    QPushButton *btn1, *btn2;

    btnStart->setIcon(btnEnd->icon());
    // put captured piece back
    switch(capturedPiece){
        case 0:
            // empty piece
            btnEnd->setIcon(QIcon());
            break;
        case 1:
            // pawn
            if ((color == BLACK) && special!=2) btnEnd->setIcon(whitePawnIcon);
            else if (special!=2) btnEnd->setIcon(blackPawnIcon);
            break;
        case 2:
            // rook
            if (color == BLACK) btnEnd->setIcon(whiteRookIcon);
            else btnEnd->setIcon(blackRookIcon);
            break;
        case 3:
            // knight
            if (color == BLACK) btnEnd->setIcon(whiteKnightIcon);
            else btnEnd->setIcon(blackKnightIcon);
            break;
        case 4:
            // bishop
            if (color == BLACK) btnEnd->setIcon(whiteBishopIcon);
            else btnEnd->setIcon(blackBishopIcon);
            break;
        case 5:
            // queen
            if (color == BLACK) btnEnd->setIcon(whiteQueenIcon);
            else btnEnd->setIcon(blackQueenIcon);
            break;
        case 6:
            // king
            if (color == BLACK) btnEnd->setIcon(whiteKingIcon);
            else btnEnd->setIcon(blackKingIcon);
            break;
    }

    // process special moves
    switch(special){
        case 1:
            // promotion
            if (color == BLACK) btnStart->setIcon(blackPawnIcon);
            else btnStart->setIcon(whitePawnIcon);
            break;
        case 2:
            // enpassant
            if (color == BLACK){
                btn1 = (QPushButton *) btnGroup.button(end-BOARDSIZE);
                btn1->setIcon(whitePawnIcon);
                btnEnd->setIcon(QIcon());
            }
            else {
                btn1 = (QPushButton *) btnGroup.button(end+BOARDSIZE);
                btn1->setIcon(blackPawnIcon);
                btnEnd->setIcon(QIcon());
            }
            break;
        case 3:
            // bqcastle
            btn1 = (QPushButton *) btnGroup.button(0);
            btn2 = (QPushButton *) btnGroup.button(3);
            btn1->setIcon(btn2->icon());
            btn2->setIcon(QIcon());
            break;
        case 4:
            // wqcastle
            btn1 = (QPushButton *) btnGroup.button(56);
            btn2 = (QPushButton *) btnGroup.button(59);
            btn1->setIcon(btn2->icon());
            btn2->setIcon(QIcon());
            break;
        case 5:
            // bkcastle
            btn1 = (QPushButton *) btnGroup.button(7);
            btn2 = (QPushButton *) btnGroup.button(5);
            btn1->setIcon(btn2->icon());
            btn2->setIcon(QIcon());
            break;
        case 6:
            // wkcastle
            btn1 = (QPushButton *) btnGroup.button(63);
            btn2 = (QPushButton *) btnGroup.button(61);
            btn1->setIcon(btn2->icon());
            btn2->setIcon(QIcon());
            break;
        default:
            // no special
            break;
    }

}

void MainWindow::enPassant(int clickOne, int clickTwo){
    // remove piece captured by enpassant

    int id;

    if (clickOne < clickTwo){
        // black performing en passant
        id = clickTwo - BOARDSIZE;
    }
    else{
        // white
        id = clickTwo + BOARDSIZE;
    }

    QPushButton *btn = (QPushButton *) btnGroup.button(id);
    btn->setIcon(QIcon());

}

void MainWindow::castle(int castleType){
    // move knight from castling

    short int from, to;

    switch (castleType)
    {
        case 3:
            // bqcastle
            from = 0;
            to = 3;
            break;
        case 4:
            // wqcastle
            from = 56;
            to = 59;
            break;
        case 5:
            // bkcastle
            from = 7;
            to = 5;
            break;
        case 6:
            // wkcastle
            from = 63;
            to = 61;
            break;
        default:
            return;
    }

    QPushButton *btnFrom = (QPushButton *) btnGroup.button(from);
    QPushButton *btnTo = (QPushButton *) btnGroup.button(to);

    btnTo->setIcon(btnFrom->icon());
    btnFrom->setIcon(QIcon());

}

bool MainWindow::eventFilter(QObject *object, QEvent *event){
    // allow icons to be resized

    int id = btnGroup.id((QAbstractButton *) object);

    if ((id != -1) && (event->type() == QEvent::Resize)){
        QPushButton *btn = (QPushButton *) btnGroup.button(id);
        QSize size = QSize(btn->size().width(), btn->size().height());
        btn->setIconSize(size);
    }
    return QMainWindow::eventFilter(object, event);

}

void MainWindow::buttonStartup(){
    // set images


    // layout information
    QLayout *layout2 = (QLayout*) layout;
    btnGroup.setExclusive(false);
    layout->setSpacing(0);

    // size policy for buttons
    QSizePolicy sizePolicy = QSizePolicy();
    sizePolicy.setHeightForWidth(true);
    sizePolicy.setWidthForHeight(true);
    sizePolicy.setHorizontalPolicy(QSizePolicy::Ignored);
    sizePolicy.setVerticalPolicy(QSizePolicy::Ignored);


    // creeete buttons
    // button values
    short int id = 0;
    short int col = 0;
    short int row = 0;

    // top row
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);


    // second row
    for (int i=0; i<BOARDSIZE; i++){
        createButton(sizePolicy, id++, 1, i);
    }

    // row 3-6 these are blank
    for (int i=2; i<6; i++){
        for (int j=0; j<BOARDSIZE; j++){
            createButton(sizePolicy, id++, i, j);
        }
    }

    // second last line
    for (int i=0; i<BOARDSIZE; i++){
        createButton(sizePolicy, id++, 6, i);
    }

    // last row
    col = 0;
    row = 7;
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);
    createButton(sizePolicy, id++, row, col++);


    //show layout
    window->setLayout(layout2);
    window->show();

}

void MainWindow::setImages(){
    // set images to default positions

    short int id = 0;

    // top row
    QPushButton *btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackRookIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackKnightIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackBishopIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackQueenIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackKingIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackBishopIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackKnightIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(blackRookIcon);


    // second row
    for (int i=0; i<BOARDSIZE; i++){
        btn = (QPushButton *) btnGroup.button(id++);
        btn->setIcon(blackPawnIcon);
    }

    // row 3-6 these are blank
    for (int i=2; i<6; i++){
        for (int j=0; j<BOARDSIZE; j++){
            btn = (QPushButton *) btnGroup.button(id++);
            btn->setIcon(QIcon());
        }
    }

    // second last line
    for (int i=0; i<BOARDSIZE; i++){
        btn = (QPushButton *) btnGroup.button(id++);
        btn->setIcon(whitePawnIcon);
    }


    // last row
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteRookIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteKnightIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteBishopIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteQueenIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteKingIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteBishopIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteKnightIcon);
    btn = (QPushButton *) btnGroup.button(id++);
    btn->setIcon(whiteRookIcon);

}

void MainWindow::createButton(QSizePolicy sizePolicy, short int id, int row, int col){
    // create button and add to layout

    // create button
    QPushButton *button = new QPushButton("",this);
    button->installEventFilter(this);
    button->setSizePolicy(sizePolicy);

    // add color
    restoreButtonColor(button, id);
    button->setAutoFillBackground(true);
    button->update();

    // add to layout and button group
    btnGroup.addButton(button,id);
    layout->addWidget(button,row,col);
}

void MainWindow::restoreButtonColor(QPushButton *btn, int id){
    // change button background color to original color

    int row = id/BOARDSIZE;
    int col = id%BOARDSIZE;
    QPalette pal = btn->palette();

    if (((col+row) % 2) == 0){
        // lighter color
        pal.setColor(QPalette::Button, lighterColor);
    }
    else{
        // darker color
        pal.setColor(QPalette::Button, darkerColor);
    }
    btn->setPalette(pal);
}

MainWindow::~MainWindow()
{

    QPushButton *btn;
    QLayout *layout2 = (QLayout*) layout;

    for (int i=0; i<BOARDSIZE*BOARDSIZE; i++){
        btn = (QPushButton *) btnGroup.button(i);
        layout2->removeWidget(btn);
        delete btn;
    }

    //delete ui;
    delete sidebar;
    delete layout;
    delete window;
}

