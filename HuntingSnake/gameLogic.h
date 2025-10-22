#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "globals.h"
#include <windows.h>
#include <time.h>
#include "thuvien.h"
#include "feature.h"

void gotoXY(int x, int y);
bool isSnakeBody(int x, int y);
int isWall(int x, int y);
bool isGate(int x, int y);
void resetSnakePosition();
int levelTargetPoints(int lv);

void maybeOpenGate();
void onEnterGate();

void generateFood();
void onAte();

void loadLevel(int lv);
void resetData();


#endif // GAME_LOGIC_H