#pragma once
#ifndef FEATURE_H
#define FEATURE_H

#include "gameLogic.h"
using namespace std;
// Hàm phụ trợ: Vẽ một ô vuông tại tọa độ logic (x, y)
void drawBlock(sf::RenderWindow& window, int x, int y, sf::Color color);
// Hàm vẽ toàn bộ game (rắn, mồi, cổng, điểm số, progress bar)
void renderGame(sf::RenderWindow& window, sf::Font& font);
#endif // FEATURE_H