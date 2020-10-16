#include "Engine.h"

Engine::Engine(){}


void Engine::engineInit()
{
    board.init();
    return;
}

bool Engine::validate(BSTR move)
{
    return board.validate(move);
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
