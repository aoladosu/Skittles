#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QLayout>
#include <Qt>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    //ui->setupUi(this);ui(new Ui::MainWindow)

    window = new QWidget();
    layout = new QGridLayout(this);

    // create buttons
    buttonStartup();
    setImages();

    // add sidebar
    sidebar = new Sidebar(this);
    layout->addWidget(sidebar, 0, 8, 8, 2);
    sidebar->showPromotion(blackBishopIcon, blackBishopIcon, blackBishopIcon, blackBishopIcon);

    // connect signals and slots
    QPushButton *back = sidebar->getBackButton();
    QPushButton *forward = sidebar->getForwardButton();
    back->setEnabled(false);
    forward->setEnabled(false);
    connect(&btnGroup, SIGNAL(idClicked(int)), this, SLOT(buttonPressed(int)));
    connect(back, SIGNAL(clicked()), this, SLOT(undoMove()));
    connect(forward, SIGNAL(clicked()), this, SLOT(redoMove()));
    connect(sidebar->getPromotionBtnGroup(), SIGNAL(idClicked(int)), this, SLOT(promotion(int)));
    connect(sidebar->getGameOverButton(), SIGNAL(clicked()), this, SLOT(newGame()));

    addLabels();

    // initialize engine
    engine.init();

    // get board value
    short int value = engine.value();
    sidebar->showValue(value);

    //show layout
    QLayout *layout2 = (QLayout*) layout;
    window->setLayout(layout2);
    window->show();
    sidebar->hidePromotion();

}

void MainWindow::buttonPressed(int id){
    // handle a button being clicked

    QPushButton *btn2 = (QPushButton *) btnGroup.button(id);
    short int special, pieceMoved, color, value, engineMove1, engineMove2;
    bool capture, check, checkmate;
    sidebar->hideError();
    value = engine.value();
    sidebar->showValue(value);


    if (firstClick == -1){
        // set first click and change button to select color
        firstClick = id;
        QPalette pal = btn2->palette();
        pal.setColor(QPalette::Button, selectColor);
        btn2->setPalette(pal);
        engine.genMovesForPiece(id, pMoves, cMoves, aMoves, true);
        highlightButtons(pMoves, pMoveColor, true);
        highlightButtons(aMoves, aMoveColor, true);
        highlightButtons(cMoves, cMoveColor, true);
        return;
    }

    if (id == firstClick){
        // unselect button, clicked twice
        firstClick = -1;
        secondClick = -1;
        restoreButtonColor(btn2, id);
        highlightButtons(pMoves, pMoveColor, false);
        highlightButtons(cMoves, cMoveColor, false);
        highlightButtons(aMoves, aMoveColor, false);
        return;
    }

    // validate move with engine
    secondClick = id;
    QPushButton *btn1 = (QPushButton *) btnGroup.button(firstClick);
    bool valid = engine.validate(firstClick, secondClick);
    restoreButtonColor(btn1, firstClick);
    highlightButtons(pMoves, pMoveColor, false);
    highlightButtons(cMoves, cMoveColor, false);
    highlightButtons(aMoves, aMoveColor, false);
    short int winner, reason;


    if (valid){
        // move was valid in chess engine, so move pieces
        special = engine.getSpecialMove();
        engine.moveStats(pieceMoved, color, capture, check, checkmate);
        // get board value
        value = engine.value();
        sidebar->showValue(value);
        handleMove(btn1, btn2, special, firstClick, secondClick, color);
        if (special != 1){
            sidebar->addMove(pieceMoved, firstClick, secondClick, color, capture, check, checkmate, special, -1);
        }
        if (engine.gameOver()){
            sidebar->hideValue();
            winner = engine.getWinner();
            reason = engine.getWinReason();
            sidebar->showGameOver(winner, reason);
        }
        sidebar->getBackButton()->setEnabled(true);
        sidebar->getForwardButton()->setEnabled(false);
        if (check){
            engine.checkPositions(chMoves);
            highlightButtons(chMoves, checkColor, true);
        }
        else{
            highlightButtons(chMoves, checkColor, false);
        }

        if (enginePlay){
            if(engineTurn){
                engineTurn = false;
            }
            else{
                engineTurn = true;
                engine.getMove(engineMove1, engineMove2);
                firstClick = engineMove1;
                buttonPressed(engineMove2);
            }
        }

    }
    else{
        short int err = engine.getErrorState();
        if ((err != 13) && (err > 3)){
            sidebar->hideValue();
            sidebar->showError(err);
        }
        else{
            value = engine.value();
            sidebar->showValue(value);
        }
    }

    firstClick = -1;
    secondClick = -1;

}

void MainWindow::promotion(int id){
    // promote piece

    short int pieceMoved, color, winner, reason, value;
    bool capture, check, checkmate;

    if (engine.promote(id)){
        if (promotionOpen) sidebar->hidePromotion();
        promotionOpen = false;
        oldSpecial = -1;
        engine.moveStats(pieceMoved, color, capture, check, checkmate);
        sidebar->addMove(pieceMoved, promotionStart, promotionPos, color, capture, check, checkmate, 1, id);

        QPushButton *btn = (QPushButton *) btnGroup.button(promotionPos);

        switch(id){
            case 2:
                // rook
                if (promotionColor == 1) btn->setIcon(blackRookIcon);
                else btn->setIcon(whiteRookIcon);
                break;
            case 3:
                // knight
                if (promotionColor == 1) btn->setIcon(blackKnightIcon);
                else btn->setIcon(whiteKnightIcon);
                break;
            case 4:
                // bishop
                if (promotionColor == 1) btn->setIcon(blackBishopIcon);
                else btn->setIcon(whiteBishopIcon);
                break;
            case 5:
                // queen
                if (promotionColor == 1) btn->setIcon(blackQueenIcon);
                else btn->setIcon(whiteQueenIcon);
                break;
        }
        if (engine.gameOver()){
            sidebar->hideValue();
            winner = engine.getWinner();
            reason = engine.getWinReason();
            sidebar->showGameOver(winner, reason);
        }
        else{
            // get board value
            value = engine.value();
            sidebar->showValue(value);
        }
    }
}

void MainWindow::newGame(){
    // start a new game


    QPushButton *btn;
    engine.init();
    setImages();
    sidebar->clearMovelist();
    sidebar->hideGameOver();
    sidebar->getBackButton()->setEnabled(false);
    sidebar->getForwardButton()->setEnabled(false);
    sidebar->hideError();
    short int value = engine.value();
    sidebar->showValue(value);
    for (short int i=0; i<BOARDSIZE*BOARDSIZE; i++){
        btn = (QPushButton *) btnGroup.button(i);
        restoreButtonColor(btn, i);
    }
}

void MainWindow::undoMove(){
    // process undoing a move

    // variables to get info back
    short int start, end, special, promoPiece, capturedPiece, color;
    engine.goBack(start, end, special, promoPiece, capturedPiece, color);
    sidebar->hideError();

    // nothing to go back to
    if (start == -1) return;

    // clear highlighting
    highlightButtons(pMoves, pMoveColor, false);
    highlightButtons(cMoves, cMoveColor, false);
    highlightButtons(aMoves, aMoveColor, false);
    highlightButtons(chMoves, checkColor, false);
    highlightButtons(chMoves, checkColor, false);

    // check if forward and back buttons should be greyed out
    handleUndoMove(start, end, special, capturedPiece, color);
    sidebar->removeMove(color);
    sidebar->hideGameOver();
    if (engine.isStart()) sidebar->getBackButton()->setEnabled(false);
    sidebar->getForwardButton()->setEnabled(true);
    QPushButton *btn = (QPushButton *) btnGroup.button(firstClick);
    if (btn != nullptr) restoreButtonColor(btn, firstClick);

    short int pieceMoved;
    bool capture, check, checkmate;
    engine.moveStats(pieceMoved, color, capture, check, checkmate);
    if (check){
        engine.checkPositions(chMoves);
        highlightButtons(chMoves, checkColor, true);
    }

    // get board value
    short int value = engine.value();
    sidebar->showValue(value);

}

void MainWindow::redoMove(){
    // process redoing a move

    // variables to get info back
    short int start, end, special, promoPiece, capturedPiece, color;
    engine.goForward(start, end, special, promoPiece, capturedPiece, color);
    sidebar->hideError();

    if (start == -1) return;

    // clear highlighting
    highlightButtons(pMoves, pMoveColor, false);
    highlightButtons(cMoves, cMoveColor, false);
    highlightButtons(aMoves, aMoveColor, false);
    highlightButtons(chMoves, checkColor, false);
    highlightButtons(chMoves, checkColor, false);



    QPushButton *btnStart = (QPushButton *) btnGroup.button(start);
    QPushButton *btnEnd = (QPushButton *) btnGroup.button(end);
    handleMove(btnStart, btnEnd, special, start, end, 1-color);
    if (promotionOpen){
        promotionOpen = false;
        sidebar->hidePromotion();
    }

    short int pieceMoved, winner, reason;
    bool capture, check, checkmate;

    engine.moveStats(pieceMoved, color, capture, check, checkmate);
    sidebar->addMove(pieceMoved, start, end, color, capture, check, checkmate, special, promoPiece);

    if (check){
        engine.checkPositions(chMoves);
        highlightButtons(chMoves, checkColor, true);
    }

    if (special == 1){
        // promotion
        QPushButton *btn = (QPushButton *) btnGroup.button(end);

        switch(promoPiece){
            case 2:
                // rook
                if (color == 1) btn->setIcon(blackRookIcon);
                else btn->setIcon(whiteRookIcon);
                break;
            case 3:
                // knight
                if (color == 1) btn->setIcon(blackKnightIcon);
                else btn->setIcon(whiteKnightIcon);
                break;
            case 4:
                // bishop
                if (color == 1) btn->setIcon(blackBishopIcon);
                else btn->setIcon(whiteBishopIcon);
                break;
            case 5:
                // queen
                if (color == 1) btn->setIcon(blackQueenIcon);
                else btn->setIcon(whiteQueenIcon);
                break;
        }
    }

    // check if this move was a game over move
    if (engine.gameOver()){
        sidebar->hideValue();
        winner = engine.getWinner();
        reason = engine.getWinReason();
        sidebar->showGameOver(winner, reason);
    }
    else{
        // get board value
        short int value = engine.value();
        sidebar->showValue(value);
    }

    // check if forward and back buttons should be greyed out
    if (engine.isEnd()) sidebar->getForwardButton()->setEnabled(false);
    sidebar->getBackButton()->setEnabled(true);


}

void MainWindow::handleMove(QPushButton *btn1, QPushButton *btn2, short int special, short int start, short int end, short int color){
    // move pieces and handle special cases

    btn2->setIcon(btn1->icon());
    btn1->setIcon(QIcon());

    switch (special) {
        case 0:
            // no special
            break;
        case 1:
            // promotion
            sidebar->hideValue();
            if (color == BLACK) sidebar->showPromotion(blackQueenIcon, blackKnightIcon, blackBishopIcon, blackRookIcon);
            else sidebar->showPromotion(whiteQueenIcon, whiteKnightIcon, whiteBishopIcon, whiteRookIcon);
            promotionOpen = true;
            promotionPos = end;
            promotionStart = start;
            promotionColor = color;
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

void MainWindow::handleUndoMove(short int start, short int end, short int special, short int capturedPiece, short int color){
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

void MainWindow::highlightButtons(short int btnList[], QColor color, bool highlight){
    // change the buttons in btnList to have color
    // or restore the buttons to their orighinal colors

    short int i = -1;
    QPushButton *btn;
    QPalette pal;

    if (highlight){
        while (btnList[++i] != -1){
            btn = (QPushButton *) btnGroup.button(btnList[i]);
            pal = btn->palette();
            pal.setColor(QPalette::Button, color);
            btn->setPalette(pal);
        }
    }
    else{
        while (btnList[++i] != -1){
            btn = (QPushButton *) btnGroup.button(btnList[i]);
            restoreButtonColor(btn, btnList[i]);
        }
    }


}

void MainWindow::enPassant(short int clickOne, short int clickTwo){
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

void MainWindow::castle(short int castleType){
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
    btnGroup.setExclusive(false);
    layout->setSpacing(0);

    // size policy for buttons
    QSizePolicy sizePolicy = QSizePolicy();
    sizePolicy.setHeightForWidth(true);
    sizePolicy.setWidthForHeight(true);
    sizePolicy.setHorizontalPolicy(QSizePolicy::Ignored);
    sizePolicy.setVerticalPolicy(QSizePolicy::Ignored);


    // create buttons
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
    for (short int i=0; i<BOARDSIZE; i++){
        createButton(sizePolicy, id++, 1, i);
    }

    // row 3-6 these are blank
    for (short int i=2; i<6; i++){
        for (short int j=0; j<BOARDSIZE; j++){
            createButton(sizePolicy, id++, i, j);
        }
    }

    // second last line
    for (short int i=0; i<BOARDSIZE; i++){
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
    for (short int i=0; i<BOARDSIZE; i++){
        btn = (QPushButton *) btnGroup.button(id++);
        btn->setIcon(blackPawnIcon);
    }

    // row 3-6 these are blank
    for (short int i=2; i<6; i++){
        for (short int j=0; j<BOARDSIZE; j++){
            btn = (QPushButton *) btnGroup.button(id++);
            btn->setIcon(QIcon());
        }
    }

    // second last line
    for (short int i=0; i<BOARDSIZE; i++){
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

void MainWindow::createButton(QSizePolicy sizePolicy, short int id, short int row, short int col){
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

void MainWindow::restoreButtonColor(QPushButton *btn, short int id){
    // change button background color to original color

    short int row = id/BOARDSIZE;
    short int col = id%BOARDSIZE;
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

void MainWindow::addLabels(){
    // add labels to board

    // size policy for buttons
    QSizePolicy sPolicy = QSizePolicy();
    sPolicy.setHorizontalPolicy(QSizePolicy::Minimum);
    sPolicy.setVerticalPolicy(QSizePolicy::Minimum);

    // create labels
    QString letters = QString("bcdefgh");
    QString numbers = QString("2345678");

    for (short int i=0; i<7; i++){
        insertLabels(sPolicy, letters.at(i), 7, i+1);
        insertLabels(sPolicy, numbers.at(i), 6-i, 0);
    }

    insertLabels(sPolicy, QString("a1"), 7, 0);

}

void MainWindow::insertLabels(QSizePolicy sPolicy, QString text, short int row, short int col){

    QLabel *label = new QLabel(QString(" ") + text, this);
    QPalette pal = label->palette();
    pal.setColor(QPalette::WindowText, QColor(202,44,106));
    label->setPalette(pal);
    label->setSizePolicy(sPolicy);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(label, row, col);
}

MainWindow::~MainWindow()
{

    QPushButton *btn;
    QLayout *layout2 = (QLayout*) layout;

    for (short int i=0; i<BOARDSIZE*BOARDSIZE; i++){
        btn = (QPushButton *) btnGroup.button(i);
        layout2->removeWidget(btn);
        delete btn;
    }

    //delete ui;
    delete sidebar;
    delete layout;
    delete window;
}

