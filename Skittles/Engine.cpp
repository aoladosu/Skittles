#include "Engine.h"

Engine::Engine(){}


void Engine::engineInit()
{
    board.init();
    return;
}

bool Engine::validate(int start, int end)
{
    return board.validate(start, end);
}

bool Engine::promote(int pieceNameVal)
{
    return board.promote(pieceNameVal);
}

int Engine::getSpecialMove()
{
    return board.getSpecial();
}

bool Engine::gameOver() {
    return board.gameOver();
}

int Engine::getWinner()
{
    return board.getWinner();
}

void Engine::goBack(int &start, int &end, int &special, int &promoPiece, int &capturedPiece){
    board.goBack(start, end, special, promoPiece, capturedPiece);
}

void Engine::goForward(int &start, int &end, int &special, int &promoPiece, int &capturedPiece){
    board.goForward(start, end, special, promoPiece, capturedPiece);
}
