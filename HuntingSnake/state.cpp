#include "globals.h"

// ========== ĐỊNH NGHĨA BIẾN ==========
POINT snake[MAX_SIZE_SNAKE];
POINT food[MAX_SIZE_FOOD];

bool SNAKE_VISIBLE[MAX_SIZE_SNAKE];

int   SIZE_SNAKE = BASE_LENGTH;
int   FOOD_INDEX = 0;
int   WIDTH_CONSOLE = 38, HEIGHT_CONSOLE = 20;
int   STATE = 0;
char  MOVING = 'D';
char  CHAR_LOCK = 'A';

int   SPEED = 3;

int   SCORE = 0;
int   LEVEL = 1;
int   LEVEL_PROGRESS = 0;
int   LEVEL_TARGET = 10;

int   GATE_ACTIVE = 0;
POINT GATE_POS = { 0,0 };
int   GATE_SIZE = 0;
POINT GATE[MAX_GATE_SIZE];
int IS_PASSING_GATE = 0;


int   FOOD_TYPE[MAX_SIZE_FOOD] = { FOOD_NORMAL };
int   FOOD_VALUE[MAX_SIZE_FOOD] = { 2 };

POINT WALLS[MAX_WALLS];
int   WALL_COUNT = 0;
