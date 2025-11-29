#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <stack>
#include <iostream>

// Include file này để kết nối Logic và View
#include "HuntingSnake/globals.h" 

#include "Design/textureManager.hpp"
#include "Design/effects.hpp"
#include "Design/interface.hpp"
#include "Design/screenState.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hunting Snake");
    window.setFramerateLimit(60); // Giới hạn FPS để game mượt mà

    // Backgrounds
    sf::Sprite start_game = loadInterface("Design/Assets/startgame.png", "start-game", window);
    sf::Sprite main_menu = loadInterface("Design/Assets/mainmenu.png", "main-menu", window);
    sf::Sprite in_game = loadInterface("Design/Assets/ingame.png", "in-game", window);
    sf::Sprite about_us = loadInterface("Design/Assets/info.png", "about-us", window);
    sf::Sprite load_game = loadInterface("Design/Assets/loadgame.png", "load-game", window);
    sf::Sprite how_to = loadInterface("Design/Assets/howto.png", "how-to", window);

    // Round
    sf::Sprite round01 = loadInterface("Design/Assets/round01.png", "round-01", window);
    sf::Sprite round02 = loadInterface("Design/Assets/round02.png", "round-02", window);
    sf::Sprite round03 = loadInterface("Design/Assets/round03.png", "hround-03", window);

    // Buttons
    Button start_button = createButton("Design/Assets/button/start.png", "", 640, 550);
    Button soundOn_button = createButton("Design/Assets/button/sound_on.png", "", 48, 48);
    Button soundOff_button = createButton("Design/Assets/button/sound_off.png", "", 48, 48);
    Button back_button = createButton("Design/Assets/button/arrow_left.png", "", 148, 48);
    Button heart_button = createButton("Design/Assets/button/heart_button.png", "", 500, 428);
    Button loadgame_button = createButton("Design/Assets/button/loadgame.png", "Design/Assets/button/loadgame_press.png", 980, 169);
    Button howto_button = createButton("Design/Assets/button/howto.png", "Design/Assets/button/howto_press.png", 980, 369);
    Button about_button = createButton("Design/Assets/button/about.png", "Design/Assets/button/about_press.png", 980, 569);

    std::vector<Button> slots;
    float x_slot = 174, y_slot = 247;
    for (int i = 0; i < 6; i++)
    {
        if (i % 2 != 0)
        {
            x_slot += 333;
            Button slot = createButton("Design/Assets/button/slot.png", "", x_slot, y_slot);
            slots.push_back(slot);
            x_slot -= 333;
        }

        else if (i % 2 == 0)
        {
            if (i != 0)
                y_slot += 150;

            Button slot = createButton("Design/Assets/button/slot.png", "", x_slot, y_slot);
            slots.push_back(slot);
        }
    }

    std::vector<Button> rocks;
    for (int i = 0; i < 3; i++)
    {
        Button rock = createButton("Design/Assets/button/rock.png", "", (720 + 140 * i), 470);
        rocks.push_back(rock);
    }

    loadgame_button.hoverOffset = sf::Vector2f(35.f, 0.f);
    howto_button.hoverOffset = sf::Vector2f(35.f, 0.f);
    about_button.hoverOffset = sf::Vector2f(35.f, 0.f);

    // Enemy and fence for round 3
    sf::Texture enemy_text;
    if (!enemy_text.loadFromFile("Design/assets/enemy.png")) {
        std::cout << "Enemy: error loading file" << std::endl;
    }
    sf::Sprite enemy_spr;
    enemy_spr.setTexture(enemy_text);

    // phần này t code một cái trước th
    sf::Texture wall_text;
    if (!enemy_text.loadFromFile("Design/assets/wall.png")) {
        std::cout << "Wall: error loading file" << std::endl;
    }
    sf::Sprite wall_spr;
    wall_spr.setTexture(wall_text);

    // Music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Design/Music/background.ogg"))
        std::cout << "Error loading background music!\n";
    else 
    {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(60);
        backgroundMusic.play();
    }

    bool isExit = false;
    bool soundOff = false;
    bool isDelete = false;
    bool isSave = false;
    bool isWin = false;
    bool isLose = false;
    bool isPause = false;

    // Initialize the screen stack once
    std::stack<ScreenState> screenStack;
    screenStack.push(ScreenState::StartMenu);

    // Font
    sf::Font font;
    if (!font.loadFromFile("Design/Assets/Font/ThaleahFat.ttf")) 
    {
        std::cout << "Error loading font!\n";
    }

    // --- CONFIG CHO GAME LOOP ---
    sf::Clock gameClock;
    float timeAccumulator = 0; // Biến tích lũy thời gian

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(70);
    timeText.setString(std::to_string(1));
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(1120, 50);

    // Clock
    sf::Clock clock;
    sf::Time duration01 = sf::seconds(30); // input thời gian ở đây nhé

    // =================== MAIN LOOP ===================
    while (window.isOpen())
    {
        // Tính thời gian trôi qua giữa các khung hình (delta time)
        float dt = gameClock.restart().asSeconds();
        timeAccumulator += dt;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                isExit = true;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                isExit = !isExit;
            if (event.type == sf::Event::KeyPressed)
            {
#ifdef _WIN32
                if (event.key.control && event.key.code == sf::Keyboard::S)
                    isSave = true;
#elif __APPLE__
                if (event.key.system && event.key.code == sf::Keyboard::S)
                    isSave = true;
#endif
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                isPause = !isPause;

            // --- XỬ LÝ INPUT RẮN (Chỉ khi đang InGame và không pause) ---
            if (!screenStack.empty() && screenStack.top() == ScreenState::InGame && !isPause) {
                if (event.type == sf::Event::KeyPressed) {
                    handleInput(event.key.code); // Hàm từ gameLogic.cpp
                }
            }
        }

        if (isExit)
        {
            window.clear();
            exitEffect(window, isExit);
            window.display();
            continue;
        }

        window.clear();
        ScreenState currentScreen = screenStack.top();

        // =================== SWITCH BETWEEN SCREENS ===================
        switch (currentScreen)
        {
        case ScreenState::StartMenu:
            window.draw(start_game);
            updateButton(start_button, window);
            drawButton(window, start_button);

            if (start_button.isClicked)
                changeScreen(screenStack, ScreenState::MainMenu);
            break;

        case ScreenState::MainMenu:
            window.draw(main_menu);

            updateButton(back_button, window);
            drawButton(window, back_button);

            updateButton(heart_button, window);
            drawButton(window, heart_button);

            updateButton(loadgame_button, window);
            drawButton(window, loadgame_button);

            updateButton(howto_button, window);
            drawButton(window, howto_button);

            updateButton(about_button, window);
            drawButton(window, about_button);

            if (back_button.isClicked)
				goBack(screenStack);

            if (howto_button.isClicked)
                changeScreen(screenStack, ScreenState::HowTo);
                
            if (about_button.isClicked)
                changeScreen(screenStack, ScreenState::About);
            
            // NÚT VÀO GAME
            if (heart_button.isClicked) {
                startGame(); // Reset dữ liệu rắn trước khi vào
                changeScreen(screenStack, ScreenState::InGame);
            }
        
            if (loadgame_button.isClicked)
                changeScreen(screenStack, ScreenState::LoadGame);
            break;

        case ScreenState::InGame:
        {
            // 1. Vẽ nền theo level
            switch (LEVEL)
            {
            case 1: window.draw(round01); break;
            case 2: window.draw(round02); break;
            case 3: window.draw(round03); break;
            default: window.draw(round01); break;
            }

            // 2. VẼ GAME (view) trước UI/modal để modal nằm trên cùng
            renderGame(window, font);

            // 3. VẼ & cập nhật nút Back (luôn cập nhật)
            updateButton(back_button, window);
            drawButton(window, back_button);

            // 4. Nếu đang hiển thị Lose popup -> xử lý Lose (ưu tiên modal lose)
            if (STATE == 0 && !isLose) {
                isLose = true;
                backgroundMusic.pause();
            }

            if (isLose) {
                int action = loseEffect(window); // 0 = nothing, 1 = replay, 2 = menu
                if (action == 1) {
                    startGame();
                    STATE = 1;
                    isLose = false;
                    isPause = false;
                    while (!screenStack.empty()) screenStack.pop();
                    screenStack.push(ScreenState::InGame);
                    backgroundMusic.play();
                    // reset clock so we don't jump frames
                    gameClock.restart();
                    timeAccumulator = 0.f;
                }
                else if (action == 2) {
                    isLose = false;
                    isPause = false;
                    STATE = 0;
                    while (!screenStack.empty()) screenStack.pop();
                    screenStack.push(ScreenState::StartMenu);
                    backgroundMusic.play();
                }
                // nếu isLose thì KHÔNG xử lý logic tiếp (modal chắn hết)
                window.draw(timeText);
                break; // thoát case để đảm bảo không vào phần logic / pause nữa
            }

            // 5. Xử lý Back button
            if (back_button.isClicked) {
				isPause = !isPause;
            }

            // 6. Nếu đang Pause -> xử lý modal Pause, và sau khi resume reset clock & skip logic
            if (isPause) {
                PauseGame(window, isPause); // trong PauseGame bạn đã set STATE=1 khi resume
                if (!isPause) {
                    // user vừa resume -> reset clocks để tránh nhảy nhiều bước
                    gameClock.restart();
                    timeAccumulator = 0.f;
                    backgroundMusic.play();
                }
                window.draw(timeText);
                break; // pause modal active: không chạy updateGameLogic trong cùng frame
            }

            // 7. CHỈ khi không pause và không lose: cập nhật logic game theo timeAccumulator
            // (đảm bảo timeAccumulator tính từ gameClock.restart() đã được reset khi resume)
            if (timeAccumulator > 0.5f / (SPEED > 0 ? SPEED : 1)) {
                timeAccumulator = 0.f;
                updateGameLogic();
            }

            // 8. UI phụ, save, win...
            window.draw(timeText);

            if (isSave) SaveGame(window, isSave);
            if (isWin) changeScreen(screenStack, ScreenState::MainMenu);

            break;
        }


        case ScreenState::About:
            window.draw(about_us);
            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);
            break;

        case ScreenState::LoadGame:
            window.draw(load_game);

            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);

            for (int i = 0; i < slots.size(); i++)
            {
                updateButton(slots[i], window);
                drawButton(window, slots[i]);
            }

            for (int i = 0; i < rocks.size(); i++)
            {
                updateButton(rocks[i], window);
                drawButton(window, rocks[i]);
            }

            if (rocks[2].isClicked)
            {
                isDelete = true;
            }

            if (rocks[1].isClicked)
            {
                changeScreen(screenStack, ScreenState::InGame); // xử lý logic ở đây nữa
            }

            if (rocks[0].isClicked)
            {
                // xử lí logic ở đây
            }

            if (isDelete)
            {
                deleteEffect(window, isDelete);
            }
            break;

        case ScreenState::HowTo:
            window.draw(how_to);
            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);
            break;

        default:
            break;
        }

        // =================== SOUND TOGGLE (GLOBAL) ===================
        // Vẽ nút âm thanh ở góc (luôn hiển thị)
        if (soundOff)
        {
            updateButton(soundOff_button, window);
            drawButton(window, soundOff_button);
            if (soundOff_button.isClicked) {
                soundOff = false;
                backgroundMusic.play();
            }
        }
        else
        {
            updateButton(soundOn_button, window);
            drawButton(window, soundOn_button);
            if (soundOn_button.isClicked) {
                soundOff = true;
                backgroundMusic.pause();
            }
        }

        // --- BẮT ĐẦU CODE DEBUG TỌA ĐỘ (Để lấy tọa độ vẽ map đè lên background: buildWalls() ---

        // 1. Lấy vị trí pixel của chuột
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

        // 2. Chuyển đổi sang tọa độ grid (dùng công thức như Cách 1)
        int gridX = (mousePixelPos.x - BOARD_X) / CELL_SIZE;
        int gridY = (mousePixelPos.y - BOARD_Y) / CELL_SIZE;

        // 3. Hiển thị lên màn hình
        sf::Text debugText;
        debugText.setFont(font);
        debugText.setCharacterSize(20); // Tăng size chữ lên chút cho dễ đọc
        debugText.setFillColor(sf::Color::Yellow); // Đổi sang màu vàng cho nổi
        debugText.setOutlineThickness(2); // Viền đen cho chữ đỡ chìm
        debugText.setOutlineColor(sf::Color::Black);

        std::string coords = "(" + std::to_string(gridX) + ", " + std::to_string(gridY) + ")";
        debugText.setString(coords);

        // 4. Đặt vị trí: Dời sang phải 15px và xuống dưới 15px so với chuột
        debugText.setPosition(mousePixelPos.x + 15, mousePixelPos.y + 15);

        window.draw(debugText);

        // --- KẾT THÚC CODE DEBUG ---

        window.display();
    }

    return 0;
}
