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

short int Engine::getWinReason(){
    return board.getWinReason();
}

bool Engine::isStart(){
    return board.isStart();
}

bool Engine::isEnd(){
    return board.isEnd();
}

short int Engine::getErrorState(){
    return board.getErrorState();
}

short int Engine::value(){
    return board.value();
}

void Engine::setAlphaBeta(bool pol){
    board.setAlphaBeta(pol);
}

void Engine::setDepth(short int depth){
    board.setDepth(depth);
}

void Engine::getMove(short int &startPos, short int &endPos){
    board.getMove(startPos, endPos);
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

bool Engine::genMovesForPiece(short int pos, short int moves[], short int cMoves[], short int aMoves[], bool extra){
    return board.genMovesForPiece(pos, moves, cMoves, aMoves, extra);
}

void Engine::checkPositions(short int pos[]){
    return board.checkPositions(pos);
}
