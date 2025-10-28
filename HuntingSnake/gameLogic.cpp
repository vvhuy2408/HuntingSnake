#include "gameLogic.h"

void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool isSnakeBody(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if (snake[i].x == x && snake[i].y == y) return true;
	}
	return false;
}

int isWall(int x, int y) {
	for (int i = 0; i < WALL_COUNT; i++)
		if (WALLS[i].x == x && WALLS[i].y == y) 
			return 1;

	return 0;
}

bool isGate(int x, int y) {
	if (!GATE_ACTIVE) return false;

	for (int i = 0; i < GATE_SIZE; i++) {
		if (GATE[i].x == x && GATE[i].y == y)
			return true;
	}
	return false;
}

void resetSnakePosition() {
	int startX = 16, startY = 5;   // đầu ở (16,5)
	for (int i = 0; i < SIZE_SNAKE; ++i) {
		snake[i] = { startX - (SIZE_SNAKE - 1 - i), startY };
	}
}

int levelTargetPoints(int lv) {
	if (lv == 1) return 10;    
	if (lv == 2) return 14;    
	if (lv == 3) return 20;
	return 30;
}

void maybeOpenGate() {
	if (GATE_ACTIVE) return;

	GATE_ACTIVE = 1;
	int x, y;
	do {
		x = 1 + rand() % (WIDTH_CONSOLE - 1);
		y = 1 + rand() % (HEIGHT_CONSOLE - 1);
	} while (isSnakeBody(x, y) || isWall(x, y));
	GATE_POS = { x,y };
	drawGate();
}

void onEnterGate() {
	eraseGate();
	// Tăng tốc độ hoặc reset length nếu đã max speed
	if (SPEED < MAX_SPEED) {
		++SPEED;
	}
	else {
		// Đã max speed → reset length về BASE_LENGTH
		SIZE_SNAKE = BASE_LENGTH;
		resetSnakePosition(); // Đặt lại vị trí rắn
	}

	loadLevel(LEVEL + 1);
}


void generateFood() {
	int x, y;
	do {
		x = 1 + rand() % (WIDTH_CONSOLE - 1);
		y = 1 + rand() % (HEIGHT_CONSOLE - 1);
	} while (isSnakeBody(x, y) || isWall(x, y) || (GATE_ACTIVE && x == GATE_POS.x && y == GATE_POS.y));

	food[FOOD_INDEX] = { x,y };

	int r = rand() % 100;
	if (LEVEL < 3) {
		FOOD_TYPE[FOOD_INDEX] = (r < 90) ? FOOD_NORMAL : FOOD_POISON;
	}
	else if (LEVEL == 3) {
		if (r < 70) FOOD_TYPE[FOOD_INDEX] = FOOD_NORMAL;
		else if (r < 90) FOOD_TYPE[FOOD_INDEX] = FOOD_BIG;
		else FOOD_TYPE[FOOD_INDEX] = FOOD_POISON;
	}
	else {
		if (r < 65) FOOD_TYPE[FOOD_INDEX] = FOOD_NORMAL;
		else if (r < 90) FOOD_TYPE[FOOD_INDEX] = FOOD_BIG;
		else FOOD_TYPE[FOOD_INDEX] = FOOD_POISON;
	}
	FOOD_VALUE[FOOD_INDEX] = (FOOD_TYPE[FOOD_INDEX] == FOOD_NORMAL ? 2 :
		FOOD_TYPE[FOOD_INDEX] == FOOD_BIG ? 6 : -2);
}

void onAte() {
	int value = FOOD_VALUE[FOOD_INDEX];
	SCORE += value;
	LEVEL_PROGRESS += value;
	if (LEVEL_PROGRESS < 0) LEVEL_PROGRESS = 0;

	if (LEVEL_PROGRESS >= LEVEL_TARGET) {
		// đủ hoặc vượt mục tiêu -> mở cổng
		LEVEL_PROGRESS = LEVEL_TARGET;          // clamp
		if (!GATE_ACTIVE) maybeOpenGate();      // tránh mở nhiều lần
	}
	else {
		// chưa đủ -> sinh mồi mới
		FOOD_INDEX = (FOOD_INDEX + 1) % MAX_SIZE_FOOD;
		generateFood(); // hoặc GenerateSingleFood()
	}

	gotoXY(0, HEIGHT_CONSOLE + 1);
	printf("Score: %d | Level: %d | Progress: %d/%d | Speed: %d\n",
		SCORE, LEVEL, LEVEL_PROGRESS, LEVEL_TARGET, SPEED);
}



void loadLevel(int lv) {
	LEVEL = lv;
	LEVEL_PROGRESS = 0;
	LEVEL_TARGET = levelTargetPoints(lv);
	GATE_ACTIVE = 0; GATE_POS = { 0,0 };
	IS_PASSING_GATE = 0;

	system("cls");
	drawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	resetSnakePosition();
	//BuildWallsForLevel(lv);   // hoặc LoadLevelFromFile(lv)
	FOOD_INDEX = 0;
	generateFood();
}

void resetData() {
	// Hướng & tốc độ ban đầu
	CHAR_LOCK = 'A';
	MOVING = 'D';
	SPEED = 3;                 // hoặc 5 nếu bạn muốn nhanh hơn
	SCORE = 0;
	IS_PASSING_GATE = 0;

	// Kích thước bàn
	WIDTH_CONSOLE = 70;
	HEIGHT_CONSOLE = 20;

	// Độ dài ban đầu
	SIZE_SNAKE = 8;

	resetSnakePosition();

	// Đánh dấu tất cả đốt đều hiển thị
	for (int i = 0; i < MAX_SIZE_SNAKE; i++) {
		SNAKE_VISIBLE[i] = true;
	}

	// Chỉ số mồi hiện tại
	FOOD_INDEX = 0;


}



