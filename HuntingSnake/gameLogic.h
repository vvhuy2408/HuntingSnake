#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "globals.h"
#include <windows.h>
#include <time.h>
#include "thuvien.h"
#include "feature.h"

// các hàm liên quan đến độ dài rắn
void moveSnake(int dx, int dy);
void growSnake();

// các hàm kiểm tra va chạm
bool isSnakeBody(int x, int y);
int isWall(int x, int y);
bool isGate(int x, int y);

// các hàm liên quan đến level, tường, cổng, mồi
void resetSnakePosition();
int levelTargetPoints(int lv);
void loadLevel(int lv);
void resetData();

// các hàm liên quan đến cổng
void maybeOpenGate();
void onEnterGate();

// các hàm liên quan đến mồi
void generateFood();
void onAte();

// hàm chính khởi động game
void startGame();
void resetData();
void updateGameLogic(); // Hàm quan trọng: cập nhật rắn mỗi frame

// hàm xử lý input
void handleInput(sf::Keyboard::Key key);

// hàm xây tường
void buildWalls(int lv);

// hàm liên quan đến enemy
void spawnEnemy(EnemyType type);
void clearEnemies();
void updateEnemySystem();
void renderEnemies(sf::RenderWindow& window, sf::Sprite& enemy_spr);
void renderBullets(sf::RenderWindow& window);
bool checkEnemyCollision();

#endif