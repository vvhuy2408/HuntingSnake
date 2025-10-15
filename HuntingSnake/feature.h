#pragma once
#ifndef FEATURE_H
#define FEATURE_H

#include "gameLogic.h"
using namespace std;
void StartGame();
void ExitGame(HANDLE t);
void PauseGame(HANDLE t);
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);
void Eat();
void ProcessDead();
void DrawSnakeAndFood(const char* str);

#endif // FEATURE_H