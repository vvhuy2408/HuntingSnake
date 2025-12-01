#pragma once
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm> 
#include "feature.h"
#include "gameLogic.h"

// ==== Cấu hình Đồ họa ====
#define CELL_SIZE 32        // Kích thước 1 ô (pixel). 32x32 là đẹp cho màn 1280x720
#define BOARD_X 40          // Tọa độ X bắt đầu lưới game (tránh đè lên UI bên trái)
#define BOARD_Y 40          // Tọa độ Y bắt đầu lưới game (tránh đè lên UI bên trên)

// ==== Config ====
#define MAX_SIZE_SNAKE    128
#define MAX_SIZE_FOOD     4
#define BASE_LENGTH       8               // MSSV 1 người
#define MAX_SNAKE_LENGTH  40              // MSSV 5 người
#define MIN_SPEED         1
#define MAX_SPEED         6
#define MAX_WALLS         1024
#define MAX_GATE_SIZE    5
#define MAX_GRID_HEIGHT 30
#define MAX_GRID_WIDTH 40

// ==== Trạng thái bàn/rắn ====
extern POINT snake[MAX_SIZE_SNAKE];
extern POINT food[MAX_SIZE_FOOD];
extern int   SIZE_SNAKE;
extern int   FOOD_INDEX;
extern int   WIDTH_CONSOLE, HEIGHT_CONSOLE;
extern int   STATE;                       // 1=Playing, 0=Pause/Over
extern char  MOVING, CHAR_LOCK;
extern bool SNAKE_VISIBLE[MAX_SIZE_SNAKE];

// ==== Tốc độ ====
extern int SPEED;

// ==== Level / Điểm / Tiến độ ====
extern int SCORE, LEVEL, LEVEL_PROGRESS, LEVEL_TARGET;

// ==== Cổng (gate) ====
extern int   GATE_ACTIVE;
extern POINT GATE_POS;
extern POINT GATE[MAX_GATE_SIZE];
extern int   GATE_SIZE;
extern int   IS_PASSING_GATE;
extern int   PASSING_JUST_STARTED;

// ==== Loại food ====
extern bool FOOD_ACTIVE;
enum { FOOD_NORMAL = 0, FOOD_POISON = 2, FOOD_BIG = 1 };
extern int FOOD_TYPE[MAX_SIZE_FOOD];      // loại của food[FOOD_INDEX]
extern int FOOD_VALUE[MAX_SIZE_FOOD];     // +2 / -2 / +6

// ==== Tường / Map (dữ liệu map do GameLogic chuẩn bị) ====
extern POINT WALLS[MAX_WALLS];
extern int   WALL_COUNT;
extern int   game_map[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];
extern sf::Font font;
