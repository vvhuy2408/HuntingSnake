#pragma once
#ifndef FEATURE_H
#define FEATURE_H

#include "gameLogic.h"
using namespace std;
void fixConsoleWindow();

void startGame();
void exitGame(HANDLE t);
void pauseGame(HANDLE t);

void drawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);
void drawSnake();
void drawFood();

void processDead();


#endif // FEATURE_H