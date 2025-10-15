#include "gameLogic.h"

void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if (snake[i].x == x && snake[i].y == y) return false;
	}
	return true;
}

void GenerateFood() {
	srand(time(NULL));
	int x, y;
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGHT_CONSOLE - 1) + 1;
		} while (!IsValid(x, y));
		food[i] = { x,y };
	}
}

void ResetData() {
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70, HEIGHT_CONSOLE = 20, SIZE_SNAKE = 6;
	snake[0] = { 10, 5 }, snake[1] = { 11, 5 }, snake[2] = { 12, 5 },
	snake[3] = { 13, 5 }, snake[4] = { 14, 5 }, snake[5] = { 15, 5 };
	GenerateFood();
}


