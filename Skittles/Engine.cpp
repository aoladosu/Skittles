#include "Engine.h"

Engine::Engine(){}


void Engine::init()
{
    board.init();
    return;
}

bool Engine::validate(short int start, short int end)
{
    return board.validate(start, end);
}

bool Engine::promote(short int pieceNameVal)
{
    return board.promote(pieceNameVal);
}

short int Engine::getSpecialMove()
{
    return board.getSpecial();
}

bool Engine::gameOver() {
    return board.gameOver();
}

short int Engine::getWinner()
{
    return board.getWinner();
}

void Engine::goBack(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color){
    board.goBack(start, end, special, promoPiece, capturedPiece, color);
}

void Engine::goForward(short int &start, short int &end, short int &special, short int &promoPiece, short int &capturedPiece, short int &color){
    board.goForward(start, end, special, promoPiece, capturedPiece, color);
}

void Engine::moveStats(short int &pieceMoved, short int &color, bool &capture, bool &check, bool &checkmate){
    board.moveStats(pieceMoved, color, capture, check, checkmate);
}
