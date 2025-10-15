#include "Feature.h"

void StartGame() {
	system("cls");
	ResetData();
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	STATE = 1;
}

void ExitGame(HANDLE t) {
    system("cls");
    TerminateThread(t, 0);
}

void PauseGame(HANDLE t) {
    if (STATE == 1) {
        STATE = 0;
        SuspendThread(t);
    }
    else if (STATE == 0) {
        STATE = 1;
        ResumeThread(t);
    }
}

void DrawBoard(int x, int y, int width, int height, int curPosX, int curPosY) {
    gotoXY(x, y); cout << 'X';
    for (int i = 1; i < width; i++) cout << 'X';
    cout << 'X';

    gotoXY(x, height + y); cout << 'X';
    for (int i = 1; i < width; i++) cout << 'X';
    cout << 'X';

    for (int i = y + 1; i < height + y; i++) {
        gotoXY(x, i); cout << 'X';
        gotoXY(x + width, i); cout << 'X';
    }

    gotoXY(curPosX, curPosY);
}

void Eat() {
    snake[SIZE_SNAKE] = food[FOOD_INDEX];
    if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
    {
        FOOD_INDEX = 0;
        SIZE_SNAKE = 6;
        if (SPEED == MAX_SPEED) SPEED = 1;
        else SPEED++;
        GenerateFood();
    }
    else {
        FOOD_INDEX++;
        SIZE_SNAKE++;
    }
}

void ProcessDead() {
    STATE = 0;
    gotoXY(0, HEIGHT_CONSOLE + 2);
    printf("Dead, type y to continue or anykey to exit");
}

void DrawSnakeAndFood(const char* str) {
    gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
    printf(str);

    for (int i = 0; i < SIZE_SNAKE; i++) {
        gotoXY(snake[i].x, snake[i].y);
        printf(str);
    }
}



