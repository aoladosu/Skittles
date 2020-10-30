#include <QLayout>
#include <QListView>
#include <QSizePolicy>
#include "Sidebar.h"


Sidebar::Sidebar(QWidget *parent) : QWidget(parent)
{

    // create layout
    gridLayout = new QGridLayout(this);

    // size policy for buttons
    QSizePolicy sizePolicy = QSizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);
    sizePolicy.setVerticalPolicy(QSizePolicy::Minimum);

    // create layout elements
    title = new QLabel(QString("Skittles"), this);

    moveList = new QListWidget(this);
    moveList->setResizeMode(QListView::Adjust);
    moveList->setViewMode(QListView::ListMode);
    moveList->setUniformItemSizes(true);
    moveList->setAlternatingRowColors(true);
    moveList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    moveList->setSizePolicy(sizePolicy);

    moveList->addItem("1. White's Move Black's Move");
    moveList->addItem("2. White's Move Black's Move");

    result = new QHBoxLayout(this);

    bottom = new QHBoxLayout(this);
    back = new QPushButton(backIcon, "", this);
    forward = new QPushButton(forwardIcon, "", this);
    QBoxLayout *bottom2 = (QBoxLayout *) bottom;
    bottom2->addWidget(back);
    bottom2->addWidget(forward);

    // add elements to layout
    gridLayout->addWidget(title,0,0,1,1);
    gridLayout->addWidget(moveList,1,0,5,0);
    gridLayout->addLayout((QLayout *) result, 6, 0, 1, 1);
    gridLayout->addLayout((QLayout *) bottom, 7, 0, 1, 1);

    // set layout
    QLayout *gridLayout2 = (QLayout *) gridLayout;
    gridLayout2->setContentsMargins(10,10,10,10);
    this->setLayout((QLayout *) gridLayout);
    this->show();

}

QPushButton* Sidebar::getBackButton(){
    return back;
}

QPushButton* Sidebar::getForwardButton(){
    return forward;
}

Sidebar::~Sidebar(){
    delete title;
    delete bottom;
    delete back;
    delete forward;
    delete result;
    delete gridLayout;
}
