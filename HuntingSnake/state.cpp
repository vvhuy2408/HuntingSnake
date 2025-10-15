#include "globals.h"

// ========== ??NH NGH?A BI?N ==========
POINT snake[MAX_SIZE_SNAKE];
POINT food[MAX_SIZE_FOOD];

int   SIZE_SNAKE = BASE_LENGTH;
int   FOOD_INDEX = 0;
int   WIDTH_CONSOLE = 70, HEIGHT_CONSOLE = 20;
int   STATE = 1;
char  MOVING = 'D';
char  CHAR_LOCK = 'A';

int   SPEED = 5;

int   SCORE = 0;
int   LEVEL = 1;
int   LEVEL_PROGRESS = 0;
int   LEVEL_TARGET = 10;

int   GATE_ACTIVE = 0;
POINT GATE_POS = { 0,0 };

int   FOOD_TYPE[MAX_SIZE_FOOD] = { FOOD_NORMAL };
int   FOOD_VALUE[MAX_SIZE_FOOD] = { 2 };

POINT WALLS[MAX_WALLS];
int   WALL_COUNT = 0;
