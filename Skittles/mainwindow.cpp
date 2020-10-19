#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QPushButton>
#include <Qt>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    //ui->setupUi(this);ui(new Ui::MainWindow)
    window = new QWidget;
    setImages();

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

void MainWindow::setImages(){
    // set images


    // layout information
    layout = new QGridLayout(this);
    QLayout *layout2 = (QLayout*) layout;
    btnGroup.setExclusive(false);
    layout->setSpacing(0);
    layout2->setContentsMargins(0,0,500,0);

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
    QPalette pal = button->palette();
    if (((col+row) % 2) == 0){
        // lighter color
        pal.setColor(QPalette::Button, QColor(237, 237, 237));
    }
    else{
        // darker color
        pal.setColor(QPalette::Button, QColor(46, 46, 46));
    }
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->update();

    // add to layout and button group
    btnGroup.addButton(button,id);
    layout->addWidget(button,row,col);
}

MainWindow::~MainWindow()
{
    //delete ui;
    delete window;
}

