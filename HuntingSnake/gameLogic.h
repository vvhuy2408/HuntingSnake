#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "globals.h"
#include <windows.h>
#include <time.h>
#include "thuvien.h"
#include "feature.h"

//void gotoXY(int x, int y);
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

// ==== Prototypes (Khai báo hàm) ====
// Logic (gameLogic.cpp)
void startGame();
void resetData();
void updateGameLogic(); // Hàm quan trọng: cập nhật rắn mỗi frame
void handleInput(sf::Keyboard::Key key);


#endif