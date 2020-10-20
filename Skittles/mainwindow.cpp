#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <Qt>
#include <QSizePolicy>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    //ui->setupUi(this);ui(new Ui::MainWindow)
    window = new QWidget;
    buttonStartup();
    connect(&btnGroup, SIGNAL(idClicked(int)), this, SLOT(buttonPressed(int)));
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
        btn2->setIcon(btn1->icon());
        btn1->setIcon(QIcon());
    }



    firstClick = -1;
    secondClick = -1;

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
    layout = new QGridLayout(this);
    QLayout *layout2 = (QLayout*) layout;
    btnGroup.setExclusive(false);
    layout->setSpacing(0);
    layout2->setContentsMargins(0,0,300,0);

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
    createButton(sizePolicy, id++, row, col++, blackRookIcon);
    createButton(sizePolicy, id++, row, col++, blackKnightIcon);
    createButton(sizePolicy, id++, row, col++, blackBishopIcon);
    createButton(sizePolicy, id++, row, col++, blackQueenIcon);
    createButton(sizePolicy, id++, row, col++, blackKingIcon);
    createButton(sizePolicy, id++, row, col++, blackBishopIcon);
    createButton(sizePolicy, id++, row, col++, blackKnightIcon);
    createButton(sizePolicy, id++, row, col++, blackRookIcon);


    // second row
    for (int i=0; i<8; i++){
        createButton(sizePolicy, id++, 1, i, blackPawnIcon);
    }

    // row 3-6 these are blank
    for (int i=2; i<6; i++){
        for (int j=0; j<8; j++){
            createButton(sizePolicy, id++, i, j);
        }
    }

    // second last line
    for (int i=0; i<8; i++){
        createButton(sizePolicy, id++, 6, i, whitePawnIcon);
    }

    // lastline
    col = 0;
    row = 7;
    createButton(sizePolicy, id++, row, col++, whiteRookIcon);
    createButton(sizePolicy, id++, row, col++, whiteKnightIcon);
    createButton(sizePolicy, id++, row, col++, whiteBishopIcon);
    createButton(sizePolicy, id++, row, col++, whiteQueenIcon);
    createButton(sizePolicy, id++, row, col++, whiteKingIcon);
    createButton(sizePolicy, id++, row, col++, whiteBishopIcon);
    createButton(sizePolicy, id++, row, col++, whiteKnightIcon);
    createButton(sizePolicy, id++, row, col++, whiteRookIcon);


    //show layout
    window->setLayout(layout2);
    window->show();

}

void MainWindow::createButton(QSizePolicy sizePolicy, short int id, int row, int col, QIcon icon){
    // create button and add to layout

    // create button
    QPushButton *button = new QPushButton(icon,"");
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

    int row = id/8;
    int col = id%8;
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
    //delete ui;
    delete layout;
    delete window;
}

