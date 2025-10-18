#include "control.h"

void moveRight() {
    if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE) {
        processDead();
    }
    else {
        if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x &&
            snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
            onAte();
        }

        for (int i = 0; i < SIZE_SNAKE - 1; i++) {
            snake[i].x = snake[i + 1].x;
            snake[i].y = snake[i + 1].y;
        }
        snake[SIZE_SNAKE - 1].x++;
    }
}

void moveLeft() {
    if (snake[SIZE_SNAKE - 1].x - 1 == 0) {
        processDead();
    }
    else {
        if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x &&
            snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
            onAte();
        }

        for (int i = 0; i < SIZE_SNAKE - 1; i++) {
            snake[i].x = snake[i + 1].x;
            snake[i].y = snake[i + 1].y;
        }
        snake[SIZE_SNAKE - 1].x--;
    }
}

void moveDown() {
    if (snake[SIZE_SNAKE - 1].y + 1 == HEIGHT_CONSOLE) {
        processDead();
    }
    else {
        if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x &&
            snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
            onAte();
        }

        for (int i = 0; i < SIZE_SNAKE - 1; i++) {
            snake[i].x = snake[i + 1].x;
            snake[i].y = snake[i + 1].y;
        }
        snake[SIZE_SNAKE - 1].y++;
    }
}

void moveUp() {
    if (snake[SIZE_SNAKE - 1].y - 1 == 0) {
        processDead();
    }
    else {
        if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x &&
            snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
            onAte();
        }

        for (int i = 0; i < SIZE_SNAKE - 1; i++) {
            snake[i].x = snake[i + 1].x;
            snake[i].y = snake[i + 1].y;
        }

        snake[SIZE_SNAKE - 1].y--;
    }
}

void threadFunc() {
    while (true) {
        if (STATE == 1) { // Snake đang sống
            // Xóa rắn cũ bằng khoảng trắng
            for (int i = 0; i < SIZE_SNAKE; ++i) {
                gotoXY(snake[i].x, snake[i].y);
                printf(" ");
            }

            // Di chuyển theo hướng hiện tại
            switch (MOVING) {
            case 'A': moveLeft();  break;
            case 'D': moveRight(); break;
            case 'W': moveUp();    break;
            case 'S': moveDown();  break;
            }

            // Vẽ lại mồi và rắn mới
            drawFood();
            drawSnake();
        }

        // delay theo tốc độ
        Sleep(1000 / SPEED);
    }
}


