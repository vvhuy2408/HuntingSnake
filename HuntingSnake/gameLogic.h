#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "globals.h"
#include <windows.h>
#include <time.h>
#include "thuvien.h"
#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 3


void FixConsoleWindow();
void gotoXY(int x, int y);
bool IsValid(int x, int y);
void GenerateFood();
void ResetData();


#endif // GAME_LOGIC_H