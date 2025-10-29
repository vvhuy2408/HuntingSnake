#include "control.h"

void threadFunc() {
    while (1) {
        if (STATE == 1) {
            // Kiểm tra SIZE_SNAKE hợp lệ
            if (SIZE_SNAKE > MAX_SIZE_SNAKE) SIZE_SNAKE = MAX_SIZE_SNAKE;
            if (SIZE_SNAKE < 0) SIZE_SNAKE = 0;

            // --- 1. Xóa rắn cũ ---
            if (!IS_PASSING_GATE) {
                for (int i = 0; i < SIZE_SNAKE; ++i) {
                    if (SNAKE_VISIBLE[i]) {
                        gotoXY(snake[i].x, snake[i].y);
                        printf(" ");
                    }
                }
            }
            else {
                // Khi qua cổng: chỉ xóa các đốt còn hiển thị
                for (int i = 0; i < SIZE_SNAKE; ++i) {
                    if (SNAKE_VISIBLE[i]) {
                        gotoXY(snake[i].x, snake[i].y);
                        printf(" ");
                    }
                }
            }

            // --- 2. Di chuyển ---
            int r = 0;
            if (SIZE_SNAKE > 0) {
                switch (MOVING) {
                case 'A': r = StepOnce(-1, 0); break;
                case 'D': r = StepOnce(1, 0); break;
                case 'W': r = StepOnce(0, -1); break;
                case 'S': r = StepOnce(0, 1); break;
                }
            }

            // --- 3. Xử lý kết quả ---
            if (r == 1) {
                onAte();
            }
            else if (r == -1) {
                processDead();
            }

            // --- 4. Xử lý QUA CỔNG ---
            if (IS_PASSING_GATE) {
                int visibleCount = 0;

                // Đếm số đốt còn hiển thị và ẩn các đốt tại vị trí cổng
                for (int i = 0; i < SIZE_SNAKE; i++) {
                    if (SNAKE_VISIBLE[i]) {
                        if (snake[i].x == GATE_POS.x && snake[i].y == GATE_POS.y) {
                            // Đốt này chạm cổng → ẩn nó
                            SNAKE_VISIBLE[i] = false;
                            gotoXY(snake[i].x, snake[i].y);
                            printf(" ");
                        }
                        else {
                            visibleCount++;
                        }
                    }
                }

                // Nếu không còn đốt nào hiển thị → qua màn
                if (visibleCount == 0) {
                    IS_PASSING_GATE = 0;

                    // Reset lại SNAKE_VISIBLE
                    for (int i = 0; i < SIZE_SNAKE; i++) {
                        SNAKE_VISIBLE[i] = true;
                    }

                    onEnterGate();
                    continue;
                }
            }

            // --- 5. Vẽ lại ---
            drawSnake();
            if (!GATE_ACTIVE && !IS_PASSING_GATE) drawFood();

            Sleep(1000 / SPEED);
        }
    }
}

int StepOnce(int dx, int dy) {
    POINT nextHead;
    nextHead.x = snake[SIZE_SNAKE - 1].x + dx;
    nextHead.y = snake[SIZE_SNAKE - 1].y + dy;

    if (isGate(nextHead.x, nextHead.y)) {
		return -1; // Dead (chạm tường cổng)
	}

    // Kiểm tra đầu rắn chạm cổng lần đầu
    if (GATE_ACTIVE && nextHead.y == GATE_POS.y && nextHead.x == GATE_POS.x) {
        IS_PASSING_GATE = 1; 
        GATE_ACTIVE = 0;     

        moveSnake(dx, dy);   
        return 0;            
    }

    if (IS_PASSING_GATE) {
        // Đang qua cổng: chỉ di chuyển, bỏ qua va chạm
        moveSnake(dx, dy);
        return 0;
    }

    // Kiểm tra va chạm với thân rắn
    if (isSnakeBody(nextHead.x, nextHead.y)) {
        return -1; // Dead
	}

    // Kiểm tra va chạm với tường
    if (isWall(nextHead.x, nextHead.y)) {
        return -1; // Dead
	}

    // Kiểm tra va chạm với biên
    if (nextHead.x <= 0 || nextHead.x >= WIDTH_CONSOLE ||
        nextHead.y <= 0 || nextHead.y >= HEIGHT_CONSOLE) {
        return -1; // Dead
    }

    // Kiểm tra ăn mồi
    if (nextHead.x == food[FOOD_INDEX].x && nextHead.y == food[FOOD_INDEX].y && SIZE_SNAKE < MAX_SIZE_SNAKE) {
        growSnake(nextHead);
        return 1;
    }

    moveSnake(dx, dy);
    return 0;
}

void growSnake(POINT nextHead) {
    snake[SIZE_SNAKE] = nextHead;
    SNAKE_VISIBLE[SIZE_SNAKE] = true;
    SIZE_SNAKE++;
}

void moveSnake(int dx, int dy) {
    for (int i = 0; i < SIZE_SNAKE - 1; i++) {
        snake[i] = snake[i + 1];
    }
    snake[SIZE_SNAKE - 1].x += dx;
    snake[SIZE_SNAKE - 1].y += dy;
}