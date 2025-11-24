#include "globals.h"
#include "gameLogic.h"
#include <cmath>

int game_map[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];
// ================= CÁC HÀM HỖ TRỢ (HELPER FUNCTIONS) =================

// Hàm di chuyển mảng rắn (Thay đổi: snake[0] là đầu)
void moveSnake(int dx, int dy) {
    // Dời thân: Đốt sau = Đốt trước (Dời từ đuôi lên đầu)
    for (int i = SIZE_SNAKE - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    // Cập nhật đầu mới
    snake[0].x += dx;
    snake[0].y += dy;
}

// Hàm tăng kích thước rắn
void growSnake() {
    // Tăng kích thước trước
    SIZE_SNAKE++;
    // Đuôi mới = Đuôi cũ (tạm thời, frame sau sẽ tự dãn ra)
    snake[SIZE_SNAKE - 1] = snake[SIZE_SNAKE - 2];
    SNAKE_VISIBLE[SIZE_SNAKE - 1] = true;
}

bool isSnakeBody(int x, int y) {
    // Kiểm tra va chạm từ đốt 1 trở đi (đốt 0 là đầu)
    for (int i = 1; i < SIZE_SNAKE; i++) {
        if (snake[i].x == x && snake[i].y == y && SNAKE_VISIBLE[i]) 
            return true;
    }
    return false;
}

int isWall(int x, int y) {
    if (x < 0 || x >= WIDTH_CONSOLE || y < 0 || y >= HEIGHT_CONSOLE) {
        return true;
    }

    if (game_map[y][x] == 1) {
        return 1;
    }

    return 0;
}

// ================= CÁC HÀM LOGIC GAME =================

void resetSnakePosition() {
    int startX = WIDTH_CONSOLE / 2; 
    int startY = HEIGHT_CONSOLE / 2;
    
    // Reset vị trí: Đầu tại startX, đuôi kéo dài về bên trái
    for (int i = 0; i < SIZE_SNAKE; ++i) {
        snake[i] = { startX - i, startY };
        SNAKE_VISIBLE[i] = true;
    }
}

void generateFood() {
    int x, y;
    do {
        x = rand() % WIDTH_CONSOLE;
        y = rand() % HEIGHT_CONSOLE;
    } while (isSnakeBody(x, y) || isWall(x, y) || (GATE_ACTIVE && x == GATE_POS.x && y == GATE_POS.y));

    food[0] = { x, y };
    
    // Random loại mồi
    int r = rand() % 100;
    if (LEVEL < 3) FOOD_TYPE[0] = (r < 90) ? 0 : 2; // 0: Normal, 2: Poison
    else {
        if (r < 60) FOOD_TYPE[0] = 0;
        else if (r < 85) FOOD_TYPE[0] = 1; // 1: Big
        else FOOD_TYPE[0] = 2;
    }
}

void buildWalls(int lv) {
    // 1. Xóa sạch tường của level cũ
    for (int y = 0; y < HEIGHT_CONSOLE; ++y) {
        for (int x = 0; x < WIDTH_CONSOLE; ++x) {
            game_map[y][x] = 0;
        }
    }

    // 2. Xây tường (collision map) cho Level 1
    if (lv == 1) {

        int top_y = 2;      // Hàng rào trên ở hàng y=2
        int bottom_y = 19;  // Hàng rào dưới ở hàng y=19
        int left_x = 0;     // Hàng rào trái ở cột x=0

        // Tọa độ của bức tường gãy khúc bên phải
        int right_wall_1_x = 28; // Tường phải (đoạn 1, x=28)
        int right_wall_2_x = 37; // Tường phải (đoạn 2, x=37)
        int right_break_y = 13;  // Chỗ gãy khúc ở hàng y=13


        // === Bắt đầu "vẽ" tường vào game_map ===
        // game_map[y][x] = 1 có nghĩa là "đây là tường"

        // Vẽ hàng rào trên (từ trái qua phải, chừa khúc bên phải)
        for (int x = left_x; x <= right_wall_1_x; x++) {
            game_map[top_y][x] = 1;
        }

        // Vẽ hàng rào trái (từ trên xuống dưới)
        for (int y = top_y; y <= bottom_y; y++) {
            game_map[y][left_x] = 1;
        }

        // Vẽ hàng rào dưới (từ trái qua phải, hết cỡ)
        for (int x = left_x; x <= right_wall_2_x; x++) {
            game_map[bottom_y][x] = 1;
        }

        // Vẽ hàng rào phải (phức tạp, 3 đoạn)
        // Đoạn 1: Từ trên (y=2) xuống chỗ gãy (y=13)
        for (int y = top_y; y <= right_break_y; y++) {
            game_map[y][right_wall_1_x] = 1;
        }
        // Đoạn 2: Ngang (từ x=28 đến x=37) tại chỗ gãy (y=13)
        for (int x = right_wall_1_x; x <= right_wall_2_x; x++) {
            game_map[right_break_y][x] = 1;
        }
        // Đoạn 3: Từ chỗ gãy (y=13) xuống dưới (y=19)
        for (int y = right_break_y; y <= bottom_y; y++) {
            game_map[y][right_wall_2_x] = 1;
        }

        // === 2.2. "Lấp" các vùng BÊN NGOÀI hàng rào (Giải pháp của bạn) ===
        // Đặt tất cả các ô bên ngoài khu vực chơi thành 1

        // 2.2.1. Lấp vùng BÊN TRÊN hàng rào
        for (int y = 0; y < top_y; ++y) {
            for (int x = 0; x < WIDTH_CONSOLE; ++x) game_map[y][x] = 1;
        }

        // 2.2.2. Lấp vùng BÊN DƯỚI hàng rào
        for (int y = bottom_y + 1; y < HEIGHT_CONSOLE; ++y) {
            for (int x = 0; x < WIDTH_CONSOLE; ++x) game_map[y][x] = 1;
        }

        // 2.2.3. Lấp vùng BÊN PHẢI (2 hình chữ nhật)
        // Vùng 1 (bên cạnh đoạn tường 1, từ y=2 đến y=13)
        for (int y = top_y; y <= right_break_y; ++y) {
            for (int x = right_wall_1_x + 1; x < WIDTH_CONSOLE; ++x) {
                game_map[y][x] = 1;
            }
        }
        // Vùng 2 (bên cạnh đoạn tường 2, từ y=14 đến y=19)
        for (int y = right_break_y + 1; y <= bottom_y; ++y) {
            for (int x = right_wall_2_x + 1; x < WIDTH_CONSOLE; ++x) {
                game_map[y][x] = 1;
            }
        }

        // (Không cần lấp bên trái vì left_x = 0 đã là tường)
    }
    // else if (lv == 2) {
    //     // Xây tường cho level 2...
    // }
}
void loadLevel(int lv) {
    LEVEL = lv;
    GATE_ACTIVE = 0;
    IS_PASSING_GATE = 0;
    resetSnakePosition();
    generateFood();
    
    buildWalls(lv);
}

void onEnterGate() {
    if (SPEED < MAX_SPEED) SPEED++;
    else SIZE_SNAKE = BASE_LENGTH; // Reset độ dài nếu max tốc độ

    loadLevel(LEVEL + 1);
}

void maybeOpenGate() {
    if (GATE_ACTIVE) return;
    GATE_ACTIVE = 1;
    
    int x, y;
    do {
        x = 2 + rand() % (WIDTH_CONSOLE - 4);
        y = 2 + rand() % (HEIGHT_CONSOLE - 4);
    } while (isSnakeBody(x, y));
    
    GATE_POS = { x, y };
}

void onAte() {
    // Xử lý điểm
    int val = (FOOD_TYPE[0] == 0) ? 1 : (FOOD_TYPE[0] == 1 ? 5 : -5);
    SCORE += val;
    if (SCORE < 0) SCORE = 0;

    // Xử lý độ dài
    if (FOOD_TYPE[0] == 2) { // Ăn độc giảm độ dài
        if (SIZE_SNAKE > BASE_LENGTH) SIZE_SNAKE--;
    } else {
        growSnake();
    }
    
    // Kiểm tra mở cổng (Ví dụ mỗi level cần 5 * LEVEL điểm để qua màn)
    if (SCORE >= LEVEL * 5 && !GATE_ACTIVE) {
        maybeOpenGate();
    } else {
        generateFood();
    }
}

void resetData() {
    SIZE_SNAKE = BASE_LENGTH;
    CHAR_LOCK = 'A';
    MOVING = 'D';
    SPEED = 3;
    SCORE = 0;
    LEVEL = 1;
    STATE = 1;
    
    // Config kích thước bàn chơi (Logic Grid)
    WIDTH_CONSOLE = 38;
    HEIGHT_CONSOLE = 20;
    
    loadLevel(1);
}

void startGame() {
    resetData();
}

// ================= UPDATE LOGIC (THAY CHO threadFunc) =================
void updateGameLogic() {
    if (STATE == 0) return;

    // 1. Xác định hướng di chuyển
    int dx = 0, dy = 0;
    switch (MOVING) {
    case 'A': dx = -1; break;
    case 'D': dx = 1; break;
    case 'W': dy = -1; break;
    case 'S': dy = 1; break;
    }

    // --- XỬ LÝ QUA CỔNG (PHẦN 1: KIỂM TRA ĐUÔI) ---
    if (IS_PASSING_GATE == 1) {
        POINT tail = snake[SIZE_SNAKE - 1];

        // Đốt cuối chạm cổng thì qua màn
        if (tail.x == GATE_POS.x && tail.y == GATE_POS.y) {
            IS_PASSING_GATE = 0; 
            onEnterGate();       
            return;
        }
    }

    // 2. Tính vị trí đầu mới dự kiến
    POINT newHead = { snake[0].x + dx, snake[0].y + dy };

    // --- XỬ LÝ QUA CỔNG (PHẦN 2: KIỂM TRA ĐẦU) ---
    // *** SỬA LỖI LOGIC: Tách riêng việc kiểm tra đầu và đuôi ***
    // Nếu cổng đang mở, VÀ rắn chưa vào cổng, VÀ đầu rắn sắp đâm vào cổng
    if (GATE_ACTIVE && IS_PASSING_GATE == 0 &&
        newHead.x == GATE_POS.x && newHead.y == GATE_POS.y) {

        IS_PASSING_GATE = 1; // Bật cờ "đang qua cổng"
        // Khi cờ này bật:
        // 1. Hàm renderGame sẽ ẩn các đốt rắn đã chui qua
        // 2. Logic va chạm bên dưới sẽ bị bỏ qua
    }

    // --- KIỂM TRA VA CHẠM (CHẾT) ---
    if (IS_PASSING_GATE == 0) {
        // 1. Đụng tường biên
        if (newHead.x < 0 || newHead.x >= WIDTH_CONSOLE ||
            newHead.y < 0 || newHead.y >= HEIGHT_CONSOLE) {
            STATE = 0; return;
        }
        // 2. Đụng thân
        if (isSnakeBody(newHead.x, newHead.y)) {
            STATE = 0; return;
        }
        // 3. Đụng tường chướng ngại vật
        if (isWall(newHead.x, newHead.y)) {
            STATE = 0; return;
        }
    }

    // --- DI CHUYỂN ---
    moveSnake(dx, dy); // Di chuyển toàn bộ thân

    // --- XỬ LÝ SAU KHI DI CHUYỂN (ĂN MỒI) ---
    if (IS_PASSING_GATE == 0) {
        if (snake[0].x == food[0].x && snake[0].y == food[0].y) {
            onAte();
        }
    }
}

void handleInput(sf::Keyboard::Key key) {
    if (STATE == 1) {
        if (key == sf::Keyboard::A && CHAR_LOCK != 'A') { MOVING = 'A'; CHAR_LOCK = 'D'; }
        else if (key == sf::Keyboard::D && CHAR_LOCK != 'D') { MOVING = 'D'; CHAR_LOCK = 'A'; }
        else if (key == sf::Keyboard::W && CHAR_LOCK != 'W') { MOVING = 'W'; CHAR_LOCK = 'S'; }
        else if (key == sf::Keyboard::S && CHAR_LOCK != 'S') { MOVING = 'S'; CHAR_LOCK = 'W'; }
    }
}