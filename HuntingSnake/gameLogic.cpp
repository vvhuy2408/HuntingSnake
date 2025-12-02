#include "gameLogic.h"
#include <cmath>
#include <vector>  
#include <algorithm> 

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

bool isGate(int x, int y) {
    if (!GATE_ACTIVE) return false;

    for (int i = 0; i < GATE_SIZE; i++) {
        if (GATE[i].x == x && GATE[i].y == y)
            return true;
    }
    return false;
}

int levelTargetPoints(int lv) {
    if (lv == 1) return 10;
    if (lv == 2) return 14;
    if (lv == 3) return 20;
    return 30;
}

void buildWalls(int lv) {
    // 1. Xóa sạch tường của level cũ
    for (int y = 0; y < HEIGHT_CONSOLE; ++y) {
        for (int x = 0; x < WIDTH_CONSOLE; ++x) {
            game_map[y][x] = 0;
        }
    }

    // 2. Xây tường (collision map) cho Level 1
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
    if (lv == 2)
    {
        //(6,8) -> (11,8); (17,8) -> (21,8); (12,14)->(17,14)
           //Hàng rào trái trên: (6,8)->(11,8)
        for (int x = 6; x <= 11; x++) {
            game_map[8][x] = 1;
        }

        //Hàng rào phải trên: (17,8)->(21,8)
        for (int x = 17; x <= 21; x++) {
            game_map[8][x] = 1;
        }

        //Hàng rào giữa dưới: (12,14)->(17,14)
        for (int x = 12; x <= 17; x++) {
            game_map[14][x] = 1;
        }
    }
    else if (lv == 3)
    {

    }

}

// ================= CÁC HÀM LOGIC GAME =================
void resetData() {
    SIZE_SNAKE = BASE_LENGTH;
    MOVING = 'A';
    CHAR_LOCK = 'D';
    SPEED = 3;
    SCORE = 0;
    LEVEL = 1;
    STATE = 1;

    IS_PASSING_GATE = 0;
    SIZE_SNAKE = 8;
    // Config kích thước bàn chơi (Logic Grid)
    WIDTH_CONSOLE = 38;
    HEIGHT_CONSOLE = 20;

    loadLevel(LEVEL);
}

void resetSnakePosition() {
    int startX = WIDTH_CONSOLE / 2 + 15;
    int startY = HEIGHT_CONSOLE / 2 + 5;

    // Reset vị trí: Đầu tại startX, đuôi kéo dài về bên trái
    for (int i = 0; i < SIZE_SNAKE; ++i) {
        snake[i] = { startX + i, startY };
        SNAKE_VISIBLE[i] = true;
    }
}

void loadLevel(int lv) {
    LEVEL = lv;
    LEVEL_TARGET = levelTargetPoints(lv);
    LEVEL_PROGRESS = 0;

    GATE_ACTIVE = 0;
    IS_PASSING_GATE = 0;
    resetSnakePosition();
    generateFood();

    buildWalls(lv);

    clearEnemies();
    
    if (lv == 3) {
        spawnEnemy(ENEMY_LEFT);   // 1 enemy bên trái
        spawnEnemy(ENEMY_BOTTOM); // 1 enemy bên dưới
    }
}

void generateFood() {

    if (GATE_ACTIVE) {
        FOOD_ACTIVE = false;
        return;
    }

    int x, y;
    do {
        x = rand() % WIDTH_CONSOLE;
        y = rand() % HEIGHT_CONSOLE;
    } while (isSnakeBody(x, y) || isWall(x, y));

    food[0] = { x, y };

    // Random loại mồi
    int r = rand() % 100;
    if (LEVEL < 3) FOOD_TYPE[0] = (r < 90) ? 0 : 2; // 0: Normal, 2: Poison
    else {
        if (r < 60) FOOD_TYPE[0] = 0;
        else if (r < 85) FOOD_TYPE[0] = 1; // 1: Big
        else FOOD_TYPE[0] = 2;
    }
    FOOD_ACTIVE = true;
}

void onAte() {
    if (GATE_ACTIVE) {
        return;
    }

    // Xử lý điểm
    int type = FOOD_TYPE[0];
    int value = (type == 0) ? 2 : (type == 1 ? 6 : -2);
    SCORE += value;
    LEVEL_PROGRESS += value;
    if (LEVEL_PROGRESS < 0) LEVEL_PROGRESS = 0;

    // Xử lý độ dài
    if (type == 2) { // Ăn độc giảm độ dài
        if (SIZE_SNAKE > BASE_LENGTH) {
            SIZE_SNAKE--;
        }
    }
    else {
        growSnake();
    }

    // Nếu đủ điểm -> mở cổng (và chắc chắn không sinh food)
    if (LEVEL_PROGRESS >= LEVEL_TARGET) {
        LEVEL_PROGRESS = LEVEL_TARGET; // clamp
        if (!GATE_ACTIVE) {
            maybeOpenGate(); // maybeOpenGate() đã xóa food và đặt FOOD_ACTIVE=false
        }
    }
    else {
        generateFood();
    }
}

void onEnterGate() {
    if (SPEED < MAX_SPEED) SPEED++;
    else SIZE_SNAKE = BASE_LENGTH; // Reset độ dài nếu max tốc độ

    loadLevel(LEVEL + 1);
}

void maybeOpenGate() {
    if (GATE_ACTIVE) return;
    GATE_ACTIVE = 1;

    if (FOOD_ACTIVE) {
        FOOD_ACTIVE = false;
    }

    int x, y;
    do {
        x = 2 + rand() % (WIDTH_CONSOLE - 4);
        y = 2 + rand() % (HEIGHT_CONSOLE - 4);
    } while (isSnakeBody(x, y) || isWall(x, y));

    GATE_POS = { x, y };
    GATE_SIZE = 0; 
    // trên cùng (3 ô) 
    GATE[GATE_SIZE++] = { x - 1, y - 1 }; 
    GATE[GATE_SIZE++] = { x, y - 1 }; 
    GATE[GATE_SIZE++] = { x + 1, y - 1 }; 
    // dưới (2 ô) 
    GATE[GATE_SIZE++] = { x - 1, y }; 
    GATE[GATE_SIZE++] = { x + 1, y };
}

void startGame() {
    resetData();
}

// Config
const float SHOOT_INTERVAL = 2.0f;  // Bắn mỗi 2 giây
const float ENEMY_SPEED = 1.5f;    // Multiplier tốc độ di chuyển enemy
const int BULLET_SPEED = 1;         // Số ô đạn di chuyển mỗi frame

// ========== LOGIC ENEMY ==========

void spawnEnemy(EnemyType type) {
    Enemy enemy;
    enemy.type = type;
    enemy.active = true;
    enemy.direction = 1;
    enemy.shootTimer = 0.0f;

    if (type == ENEMY_LEFT) {
        // Bắt đầu ở giữa tường trái
        enemy.pos.x = 1; // Cách tường 1 ô (tường ở x=0)
        enemy.pos.y = HEIGHT_CONSOLE / 2;
    }
    else { // ENEMY_BOTTOM
        // Bắt đầu ở giữa tường dưới
        enemy.pos.x = WIDTH_CONSOLE / 2;
        enemy.pos.y = HEIGHT_CONSOLE - 2; // Cách tường 1 ô (tường ở y=19)
    }

    enemies.push_back(enemy);
}

void clearEnemies() {
    enemies.clear();
    bullets.clear();
}

void moveEnemy(Enemy& enemy) {
    if (enemy.type == ENEMY_LEFT) {
        // Di chuyển dọc (lên xuống)
        enemy.pos.y += enemy.direction;

        // Kiểm tra va chạm tường -> đổi hướng
        if (enemy.pos.y <= 3 || enemy.pos.y >= HEIGHT_CONSOLE - 2) {
            enemy.direction *= -1;
            enemy.pos.y += enemy.direction; // Điều chỉnh lại vị trí
        }
    }
    else { // ENEMY_BOTTOM
        // Di chuyển ngang (trái phải)
        enemy.pos.x += enemy.direction;

        // Kiểm tra va chạm tường -> đổi hướng
        if (enemy.pos.x <= 1 || enemy.pos.x >= WIDTH_CONSOLE - 2) {
            enemy.direction *= -1;
            enemy.pos.x += enemy.direction;
        }
    }
}

void shootBullet(const Enemy& enemy) {
    Bullet bullet;
    bullet.pos = enemy.pos;
    bullet.active = true;

    if (enemy.type == ENEMY_LEFT) {
        // Bắn sang phải
        bullet.dx = 1;
        bullet.dy = 0;
    }
    else { // ENEMY_BOTTOM
        // Bắn lên trên
        bullet.dx = 0;
        bullet.dy = -1;
    }

    bullets.push_back(bullet);
}

void updateEnemies(float dt) {
    if (LEVEL < 3) return;

    for (auto& enemy : enemies) {
        if (!enemy.active) continue;

        // Cập nhật timer bắn
        enemy.shootTimer += dt;

        if (enemy.shootTimer >= SHOOT_INTERVAL) {
            shootBullet(enemy);
            enemy.shootTimer = 0.0f;
        }
    }
}

void moveEnemies() {
    if (LEVEL < 3) return;

    POINT head = snake[0]; // Lưu vị trí đầu rắn

    for (auto& enemy : enemies) {
        if (!enemy.active) continue;

        // Kiểm tra va chạm TRƯỚC KHI di chuyển
        if (enemy.pos.x == head.x && enemy.pos.y == head.y) {
            STATE = 0;
            return;
        }

        moveEnemy(enemy);

        // Kiểm tra va chạm SAU KHI di chuyển
        if (enemy.pos.x == head.x && enemy.pos.y == head.y) {
            STATE = 0;
            return;
        }
    }
}

// ========== HÀM LOGIC ĐẠN ==========

void updateBullets() {
    POINT head = snake[0]; // Lưu vị trí đầu rắn

    for (auto& bullet : bullets) {
        if (!bullet.active) continue;

        // Kiểm tra va chạm TRƯỚC KHI di chuyển
        if (SNAKE_VISIBLE[0] &&
            bullet.pos.x == head.x &&
            bullet.pos.y == head.y) {
            STATE = 0;
            bullet.active = false;
            return;
        }

        // Di chuyển đạn
        bullet.pos.x += bullet.dx * BULLET_SPEED;
        bullet.pos.y += bullet.dy * BULLET_SPEED;

        // Kiểm tra va chạm Sau khi di chuyển
        if (SNAKE_VISIBLE[0] &&
            bullet.pos.x == head.x &&
            bullet.pos.y == head.y) {
            STATE = 0;
            bullet.active = false;
            return;
        }

        // Kiểm tra đạn ra khỏi màn hình
        if (bullet.pos.x < 0 || bullet.pos.x >= WIDTH_CONSOLE ||
            bullet.pos.y < 0 || bullet.pos.y >= HEIGHT_CONSOLE) {
            bullet.active = false;
            continue;
        }

        // Kiểm tra đạn đụng tường
        if (isWall(bullet.pos.x, bullet.pos.y)) {
            bullet.active = false;
            continue;
        }
    }

    // Xóa đạn không active
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.active; }),
        bullets.end()
    );
}

bool checkEnemyCollision() {
    if (LEVEL < 3) return false;

    POINT head = snake[0];

    for (const auto& enemy : enemies) {
        if (!enemy.active) continue;

        if (enemy.pos.x == head.x && enemy.pos.y == head.y) {
            return true; // Va chạm -> Game Over
        }
    }
    return false;
}

// ========== HÀM CHÍNH CẬP NHẬT ==========

void updateEnemySystem() {
    if (LEVEL < 3) return;

    static sf::Clock enemyClock;
    static float enemyAccumulated = 0.0f;

    float dt = enemyClock.restart().asSeconds();
    if (dt > 0.5f) dt = 0.5f;

    enemyAccumulated += dt;

    // Cập nhật timer bắn
    updateEnemies(dt);

    // Di chuyển enemy theo tốc độ rắn
    float snakeInterval = 0.5f / (SPEED > 0 ? SPEED : 1);
    float enemyInterval = snakeInterval * ENEMY_SPEED;

    if (enemyAccumulated >= enemyInterval) {
        enemyAccumulated -= enemyInterval;
        moveEnemies();
    }

    // Cập nhật đạn (chạy mỗi frame)
    updateBullets();

    // Kiểm tra va chạm
    if (checkEnemyCollision()) {
        STATE = 0;
    }
}

// ========== RENDER ENEMY & BULLETS ==========
//hàm này được gọi bên feature nhé
void renderEnemies(sf::RenderWindow& window, sf::Sprite& enemy_spr) {
    for (const auto& enemy : enemies) {
        if (!enemy.active) continue;

        if (enemy_spr.getTexture()) {
            sf::Sprite spr = enemy_spr;

            // Scale đều
            float scale = (float)CELL_SIZE / enemy_spr.getTexture()->getSize().x;

            if (enemy.type == ENEMY_LEFT) {
                // enemy trái: LẬT NGANG 180 độ (reflect horizontal)
                spr.setScale(-scale, scale);  // Scale âm = lật ngang

                // Khi lật, origin thay đổi nên phải điều chỉnh vị trí
                spr.setPosition(
                    BOARD_X + enemy.pos.x * CELL_SIZE + CELL_SIZE, 
                    BOARD_Y + enemy.pos.y * CELL_SIZE
                );
            }
            else { // ENEMY_BOTTOM
                // enemy dưới: XOAY 90 độ THEO CHIỀU KIM ĐỒNG HỒ
                spr.setRotation(90);  // Xoay 90 độ
                spr.setScale(scale, scale);

                // Sau khi xoay, phải điều chỉnh vị trí
                spr.setPosition(
                    BOARD_X + enemy.pos.x * CELL_SIZE + CELL_SIZE,
                    BOARD_Y + enemy.pos.y * CELL_SIZE
                );
            }

            window.draw(spr);
        }
    }
}

void renderBullets(sf::RenderWindow& window) {
    for (const auto& bullet : bullets) {
        if (!bullet.active) continue;

        // Vẽ đạn bằng hình tròn đỏ
        sf::CircleShape bulletShape(CELL_SIZE / 4.0f);
        bulletShape.setFillColor(sf::Color::Red);
        bulletShape.setPosition(
            BOARD_X + bullet.pos.x * CELL_SIZE + CELL_SIZE / 4.0f,
            BOARD_Y + bullet.pos.y * CELL_SIZE + CELL_SIZE / 4.0f
        );

        window.draw(bulletShape);
    }
}

// ================= UPDATE LOGIC (THAY CHO threadFunc) =================
void updateGameLogic() {
    if (STATE == 0) return;

    updateEnemySystem();

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
        /*if (newHead.x < 0 || newHead.x >= WIDTH_CONSOLE ||
            newHead.y < 0 || newHead.y >= HEIGHT_CONSOLE) {
            STATE = 0; return;
        }*/
        // 2. Đụng thân
        if (isSnakeBody(newHead.x, newHead.y)) {
            STATE = 0; return;
        }
        // 3. Đụng tường chướng ngại vật
        if (isWall(newHead.x, newHead.y)) {
            STATE = 0; return;
        }
        if (isGate(newHead.x, newHead.y)) {
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