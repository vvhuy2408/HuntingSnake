#include "Feature.h"

void fixConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void HideConsoleCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = FALSE;      // ẩn
    SetConsoleCursorInfo(hOut, &cci);
}

void ShowConsoleCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = TRUE;       // hiện lại
    SetConsoleCursorInfo(hOut, &cci);
}

void startGame() {
    system("cls");
    srand((unsigned)time(NULL));          

    resetData();

    loadLevel(1);

    drawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);

    STATE = 1;
}


void exitGame(HANDLE t) {
    system("cls");
    TerminateThread(t, 0);
}

void pauseGame(HANDLE t) {
    if (STATE == 1) {
        STATE = 0;
        SuspendThread(t);
    }
    else if (STATE == 0) {
        STATE = 1;
        ResumeThread(t);
    }
}

void drawBoard(int x, int y, int width, int height, int curPosX, int curPosY) {
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

void drawGate() {
    if (!GATE_ACTIVE) return;

    int x = GATE_POS.x;
    int y = GATE_POS.y;
    GATE_SIZE = 0;

    // trên cùng (3 ô)
    GATE[GATE_SIZE++] = { x - 1, y - 1 };
    GATE[GATE_SIZE++] = { x,     y - 1 };
    GATE[GATE_SIZE++] = { x + 1, y - 1 };

    // dưới (2 ô)
    GATE[GATE_SIZE++] = { x - 1, y };
    GATE[GATE_SIZE++] = { x + 1, y };

    // in ra màn hình
    for (int i = 0; i < GATE_SIZE; i++) {
        gotoXY(GATE[i].x, GATE[i].y);
        printf("@");  // ký tự thể hiện cổng
    }
}


void eraseGate() {
    int x = GATE_POS.x;
    int y = GATE_POS.y;

    for (int dy = -1; dy <= 0; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (!(dy == 0 && dx == 0)) { // bỏ điểm giữa nếu bạn muốn rỗng
                gotoXY(x + dx, y + dy);
                printf(" ");
            }
        }
    }
    GATE_SIZE = 0;
}


void processDead() {
    STATE = 0;
    gotoXY(0, HEIGHT_CONSOLE + 2);
    printf("Dead, type y to continue or anykey to exit");
}

void drawSnake() {
    static const char* MSSV_SEQ = "2412711124127406241275102412752224127550";
    int L = (int)strlen(MSSV_SEQ);

    for (int i = 0; i < SIZE_SNAKE; ++i) {
        // khoảng cách từ đầu về đốt hiện tại
        int idx = (SIZE_SNAKE - 1 - i) % L;   // i = SIZE_SNAKE-1 (đầu) -> idx = 0
        char c = MSSV_SEQ[idx];

        gotoXY(snake[i].x, snake[i].y);
        putchar(c);
    }
}


// O = thường, Q = big, X = độc
void drawFood() {
    gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
    switch (FOOD_TYPE[FOOD_INDEX]) {
    case FOOD_NORMAL: printf("O"); break;
    case FOOD_BIG:    printf("Q"); break;
    case FOOD_POISON: printf("X"); break;
    default:          printf("O"); break;
    }
}




