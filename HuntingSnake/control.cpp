#include "control.h"

void threadFunc() {
    while (1) {
        if (STATE == 1) { // If my snake is alive
            // Xóa rắn cũ bằng khoảng trắng
            for (int i = 0; i < SIZE_SNAKE; ++i) {
                gotoXY(snake[i].x, snake[i].y);
                printf(" ");
            }

            int r;
            switch (MOVING) {
            case 'A':
                r = StepOnce(-1, 0);
                break;
            case 'D':
                r = StepOnce(1, 0);
                break;
            case 'W':
                r = StepOnce(0, -1);
                break;
            case 'S':
                r = StepOnce(0, 1);
                break;
            }

            if (r == 1) {
                
                onAte();
			}
            else if (r == 2) onEnterGate();
			else if (r == -1) processDead();

            drawSnake();
            drawFood();

            // delay theo tốc độ
            Sleep(1000 / SPEED);
        }
    }
}

int StepOnce(int dx, int dy) {
    POINT nextHead;
	nextHead.x = snake[SIZE_SNAKE - 1].x + dx;
	nextHead.y = snake[SIZE_SNAKE - 1].y + dy;

    //is snake dead?
    for (int i = 0; i < SIZE_SNAKE - 1; i++) {
        if (snake[i].x == nextHead.x &&
            snake[i].y == nextHead.y) {
            return -1; // Dead
        }
    }
    for (int i = 0; i < WALL_COUNT; i++) {
        if (WALLS[i].x == nextHead.x &&
            WALLS[i].y == nextHead.y) {
            return -1; // Dead
        }
    }
    if (nextHead.x <= 0 || nextHead.x >= WIDTH_CONSOLE ||
        nextHead.y <= 0 || nextHead.y >= HEIGHT_CONSOLE) {
        return -1; // Dead
    }

    if (nextHead.y == GATE_POS.y && nextHead.x == GATE_POS.x) return 2; // Reached the gate

    if (nextHead.x == food[FOOD_INDEX].x && nextHead.y == food[FOOD_INDEX].y && SIZE_SNAKE < MAX_SIZE_SNAKE) {
		//increase size of snake
        growSnake(nextHead);
        return 1;
    }
	moveSnake(dx, dy);
    return 0;
}

void growSnake(POINT nextHead) {
	snake[SIZE_SNAKE] = nextHead;
    SIZE_SNAKE++;
}

void moveSnake(int dx, int dy) {
    for (int i = 0; i < SIZE_SNAKE - 1; i++) {
        snake[i] = snake[i + 1];
	}
	snake[SIZE_SNAKE - 1].x += dx;
	snake[SIZE_SNAKE - 1].y += dy;
}