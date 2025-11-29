#include "globals.h"
#include "feature.h"

// Hàm phụ trợ: Vẽ một ô vuông tại tọa độ logic (x, y)
// Giúp code gọn hơn, không phải setPosition lặp lại nhiều lần
void drawBlock(sf::RenderWindow& window, int x, int y, sf::Color color) {
    // Trừ đi 2 pixel để tạo khe hở nhỏ giữa các ô, nhìn đẹp hơn
    sf::RectangleShape block(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    block.setFillColor(color);

    // Công thức: Tọa độ màn hình = (Tọa độ Logic * Kích thước ô) + Lề Bàn Cờ + Offset viền
    block.setPosition(BOARD_X + x * CELL_SIZE + 1, BOARD_Y + y * CELL_SIZE + 1);

    window.draw(block);
}

void renderGame(sf::RenderWindow& window, sf::Font& font) {

    // 1.1 VẼ ĐIỂM SỐ (SCORE) - Vẽ vào vị trí POINT trên background
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setString(std::to_string(SCORE));
    // Căn vào vị trí "(34, 8)" trên background - điều chỉnh tọa độ pixel cho khớp
    int gridX = 34;
    int gridY = 8;
    int scoreX = gridX * CELL_SIZE + BOARD_X;
    int scoreY = gridY * CELL_SIZE + BOARD_Y;
    scoreText.setPosition(scoreX, scoreY); // Điều chỉnh vị trí này cho khớp với "POINT" trong ảnh
    window.draw(scoreText);

    // 1.2 VẼ PROGRESS BAR
    // Vị trí và kích thước thanh progress
    int barX = 34 * CELL_SIZE + BOARD_X;  // Cùng cột với SCORE
    int barY = 7 * CELL_SIZE + BOARD_Y;   // Ở trên POINT một chút
    int barWidth = 130;                    // Chiều rộng thanh
    int barHeight = 20;                    // Chiều cao thanh
    
    // Vẽ khung thanh (nền)
    sf::RectangleShape barBackground(sf::Vector2f(barWidth, barHeight));
    barBackground.setPosition(barX, barY);
    barBackground.setFillColor(sf::Color(50, 50, 50)); // Màu xám đậm
    barBackground.setOutlineThickness(2);
    barBackground.setOutlineColor(sf::Color::White);
    window.draw(barBackground);
    
    // Vẽ thanh tiến độ (dựa trên SCORE, max = 30)
    float progressPercent = std::min(SCORE / 30.0f, 1.0f); // Tỷ lệ 0.0 -> 1.0
    int fillWidth = static_cast<int>(barWidth * progressPercent);
    
    sf::RectangleShape barFill(sf::Vector2f(fillWidth, barHeight));
    barFill.setPosition(barX, barY);
    
    // Đổi màu theo tiến độ: xanh lục -> vàng -> đỏ
    if (progressPercent < 0.5f) {
        barFill.setFillColor(sf::Color::Green);
    } else if (progressPercent < 0.8f) {
        barFill.setFillColor(sf::Color::Yellow);
    } else {
        barFill.setFillColor(sf::Color::Red);
    }
    window.draw(barFill);

    // 2. VẼ CỔNG (GATE)
    // Chỉ vẽ khi cổng đang kích hoạt
    if (GATE_ACTIVE) {
        sf::Color gateColor = sf::Color::Cyan; // Màu xanh lơ cho cổng
        int gx = GATE_POS.x;
        int gy = GATE_POS.y;

        // Vẽ cổng theo hình dáng chữ U ngược (như logic cũ)
        // Hàng trên (3 ô)
        drawBlock(window, gx - 1, gy - 1, gateColor);
        drawBlock(window, gx, gy - 1, gateColor);
        drawBlock(window, gx + 1, gy - 1, gateColor);

        // Hàng dưới (2 chân)
        drawBlock(window, gx - 1, gy, gateColor);
        drawBlock(window, gx + 1, gy, gateColor);
    }

    if (FOOD_ACTIVE && !GATE_ACTIVE) {
    // 3. VẼ THỨC ĂN (FOOD)
        sf::Color foodColor;
        // 0: Normal (Đỏ), 1: Big (Tím), 2: Poison (Xanh lá độc/Vàng)
        if (FOOD_TYPE[0] == 0)      foodColor = sf::Color::Red;
        else if (FOOD_TYPE[0] == 1) foodColor = sf::Color::Magenta;
        else                        foodColor = sf::Color(200, 200, 0); // Màu vàng tối cho thuốc độc

        // Vẽ mồi hình tròn để khác biệt với rắn
        sf::CircleShape foodShape(CELL_SIZE / 2.0f - 2);
        foodShape.setFillColor(foodColor);
        foodShape.setPosition(BOARD_X + food[0].x * CELL_SIZE + 2, BOARD_Y + food[0].y * CELL_SIZE + 2);
        window.draw(foodShape);
    }
    
    // 4. VẼ RẮN (SNAKE)
    std::string mssv_digits = "2412711124127406241275102412752224127550";

    // Tạo một đối tượng Text (chỉ 1 lần là đủ)
    sf::Text mssv_text;
    mssv_text.setFont(font);
    // Đặt kích thước chữ nhỏ hơn ô, ví dụ 70% kích thước ô
    mssv_text.setCharacterSize(static_cast<unsigned int>(CELL_SIZE * 0.7));

    for (int i = 0; i < SIZE_SNAKE; ++i) {
        if (IS_PASSING_GATE) {
            int gx = GATE_POS.x;
            int gy = GATE_POS.y; // Đây là tọa độ "miệng" cổng (ô trống)

            // Cổng hình chữ U ngược có 3 ô trên cùng ở hàng (gy - 1).
            // "Bên trong" cổng là khi y <= gy - 1.
            // Chúng ta ẩn bất kỳ đốt rắn nào có tọa độ y <= (gy - 1)
            // (Tức là đã chui vào HOẶC chui qua hàng trên cùng của cổng)
            // Logic này giả định rắn luôn đi LÊN (y giảm) để qua cổng.
            if (snake[i].y <= gy - 1) {
                continue; // Bỏ qua, không vẽ đốt này
            }
        }

        if (!SNAKE_VISIBLE[i]) continue;

        sf::Color bodyColor;
        if (i == 0) {
            bodyColor = sf::Color(255, 0, 128); // Đầu rắn: Hồng cute
        }
        else {
            bodyColor = (i % 2 == 0) ? sf::Color(0, 200, 0) : sf::Color(0, 160, 0);
        }

        drawBlock(window, snake[i].x, snake[i].y, bodyColor);

        // *** 4.B. VẼ MSSV LÊN TRÊN ĐỐT RẮN ***

        // Lấy ký tự tại vị trí 'i', nếu rắn dài hơn chuỗi thì lặp lại từ đầu
        char digit_char = mssv_digits[i % mssv_digits.length()];
        std::string digit_string(1, digit_char); // Chuyển char thành string

        mssv_text.setString(digit_string);

        // Đổi màu text để tương phản (ví dụ: đầu rắn màu sáng thì text màu đen)
        if (i == 0) {
            mssv_text.setFillColor(sf::Color::Black);
        }
        else {
            mssv_text.setFillColor(sf::Color::White);
        }

        // Căn giữa chữ vào giữa ô
        sf::FloatRect textBounds = mssv_text.getLocalBounds();
        mssv_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
            textBounds.top + textBounds.height / 2.0f);

        mssv_text.setPosition(
            BOARD_X + snake[i].x * CELL_SIZE + CELL_SIZE / 2.0f,
            BOARD_Y + snake[i].y * CELL_SIZE + CELL_SIZE / 2.0f
        );

        window.draw(mssv_text);
    }
}