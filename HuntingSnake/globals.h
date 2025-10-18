#pragma once
#include <windows.h>

// ==== Config ====
#define MAX_SIZE_SNAKE    128
#define MAX_SIZE_FOOD     4
#define BASE_LENGTH       8               // MSSV 1 người
#define MAX_SNAKE_LENGTH  40              // MSSV 5 người
#define MIN_SPEED         1
#define MAX_SPEED         20
#define MAX_WALLS         1024

// ==== Trạng thái bàn/rắn ====
extern POINT snake[MAX_SIZE_SNAKE];
extern POINT food[MAX_SIZE_FOOD];
extern int   SIZE_SNAKE;
extern int   FOOD_INDEX;
extern int   WIDTH_CONSOLE, HEIGHT_CONSOLE;
extern int   STATE;                       // 1=Playing, 0=Pause/Over
extern char  MOVING, CHAR_LOCK;

// ==== Tốc độ ====
extern int SPEED;

// ==== Level / Điểm / Tiến độ ====
extern int SCORE, LEVEL, LEVEL_PROGRESS, LEVEL_TARGET;

// ==== Cổng (gate) ====
extern int   GATE_ACTIVE;
extern POINT GATE_POS;

// ==== Loại food ====
enum { FOOD_NORMAL = 0, FOOD_POISON = 2, FOOD_BIG = 1 };
extern int FOOD_TYPE[MAX_SIZE_FOOD];      // loại của food[FOOD_INDEX]
extern int FOOD_VALUE[MAX_SIZE_FOOD];     // +2 / -2 / +6

// ==== Tường / Map (dữ liệu map do GameLogic chuẩn bị) ====
extern POINT WALLS[MAX_WALLS];
extern int   WALL_COUNT;
