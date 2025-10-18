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
}

void onEnterGate() {
	if (!GATE_ACTIVE) return;
	GATE_ACTIVE = 0;

	if (SPEED < MAX_SPEED) ++SPEED;
	else SIZE_SNAKE = BASE_LENGTH; // đang max speed → reset length về 7 (theo luật bạn đặt)

	//LoadLevel(LEVEL + 1);
	// vẽ sẽ do Feature gọi RenderAll() sau LoadLevel
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
		FOOD_TYPE[FOOD_INDEX] = (r < 80) ? FOOD_NORMAL : FOOD_POISON;
	}
	else {
		if (r < 70) FOOD_TYPE[FOOD_INDEX] = FOOD_NORMAL;
		else if (r < 85) FOOD_TYPE[FOOD_INDEX] = FOOD_POISON;
		else FOOD_TYPE[FOOD_INDEX] = FOOD_BIG;
	}
	FOOD_VALUE[FOOD_INDEX] = (FOOD_TYPE[FOOD_INDEX] == FOOD_NORMAL ? 2 :
		FOOD_TYPE[FOOD_INDEX] == FOOD_BIG ? 6 : -2);
}

void onAte() {
	// 1Cập nhật điểm và tiến độ level
	int value = FOOD_VALUE[FOOD_INDEX];
	SCORE += value;
	LEVEL_PROGRESS += value;

	// Không để tiến độ âm (nếu ăn mồi độc)
	if (LEVEL_PROGRESS < 0)
		LEVEL_PROGRESS = 0;

	// 2Nếu chưa đủ điểm để mở cổng → sinh mồi mới
	if (LEVEL_PROGRESS < LEVEL_TARGET) {
		// chuyển sang food kế tiếp (vòng tròn)
		FOOD_INDEX = (FOOD_INDEX + 1) % MAX_SIZE_FOOD;
		generateFood();
	}

	if (LEVEL_PROGRESS > LEVEL_TARGET) 
		LEVEL_PROGRESS = LEVEL_TARGET;

	// Nếu đã đủ điểm → mở cổng
	else {
		maybeOpenGate();
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

	//BuildWallsForLevel(lv);   // hoặc LoadLevelFromFile(lv)
	FOOD_INDEX = 0;
	generateFood();
}

void resetData() {
	// Hướng & tốc độ ban đầu
	CHAR_LOCK = 'A';
	MOVING = 'D';
	SPEED = 3;                 // hoặc 5 nếu bạn muốn nhanh hơn

	// Kích thước bàn
	WIDTH_CONSOLE = 70;
	HEIGHT_CONSOLE = 20;

	// Độ dài ban đầu
	SIZE_SNAKE = 8;

	// Khởi tạo rắn 7 đốt nằm ngang, đầu ở snake[SIZE_SNAKE-1]
	int startX = 16, startY = 5;   // đầu ở (16,5)
	for (int i = 0; i < SIZE_SNAKE; ++i) {
		snake[i] = { startX - (SIZE_SNAKE - 1 - i), startY };
	}

	// Chỉ số mồi hiện tại
	FOOD_INDEX = 0;

}



