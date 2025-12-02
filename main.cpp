#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <stack>
#include <iostream>

// Include file này để kết nối Logic và View
#include "HuntingSnake/globals.h" 
#include "HuntingSnake/save_load.h"
#include "Design/textureManager.hpp"
#include "Design/effects.hpp"
#include "Design/interface.hpp"
#include "Design/screenState.hpp"
#include "HuntingSnake/feature.h"

// Biến global liên quan đến save/load
std::vector<SaveFileInfo> saveFileInfos;
std::vector<sf::Text> saveFileTexts;
int selectedSlot = -1;

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

	// Save slots and rocks
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

    enemy_spr.setTexture(enemy_text);

	// Wall for round 2
    sf::Texture wall_text;
    if (!wall_text.loadFromFile("Design/assets/wall.png")) {
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

    bool soundOff = false;

	bool isModalOpen = false; // flag kiểm tra có modal đang mở không
    bool isExit = false;
    bool isPause = false;

    bool isDelete = false;
    bool isSave = false;
    bool isSaving = false; // đang xử lý save, tạm dừng game
    bool isRename = false;
    std::string renameOutput = "";

    bool isWin = false;
    bool isLose = false;

    // Initialize the screen stack once
    std::stack<ScreenState> screenStack;
    screenStack.push(ScreenState::StartMenu);

    // Font
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

        // Cập nhật flag modal
        isModalOpen = (isSaving || isRename || isDelete || isLose || isPause);

		// Xử lý sự kiện cửa sổ
        sf::Event event;
        while (window.pollEvent(event))
        {   
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            // === CHỈ XỬ LÝ EVENTS KHI KHÔNG CÓ MODAL ===
            if (!isModalOpen) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    isExit = !isExit;
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::L)
                        isSave = true;
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                    isPause = !isPause;

                // XỬ LÝ INPUT RẮN
                if (!screenStack.empty() && screenStack.top() == ScreenState::InGame && !isPause) {
                    if (event.type == sf::Event::KeyPressed) {
                        handleInput(event.key.code);
                    }
                }
            }
        }
		// exit effect
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
			// =================== START MENU ===================
        case ScreenState::StartMenu:
            window.draw(start_game);
            updateButton(start_button, window);
            drawButton(window, start_button);

            if (start_button.isClicked)
                changeScreen(screenStack, ScreenState::MainMenu);
            break;
			// =================== MAIN MENU ===================
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

			// NÚT BACK
            if (back_button.isClicked)
				goBack(screenStack);

			// NÚT HOW TO
            if (howto_button.isClicked)
                changeScreen(screenStack, ScreenState::HowTo);

			// NÚT ABOUT
            if (about_button.isClicked)
                changeScreen(screenStack, ScreenState::About);
            
            // NÚT VÀO GAME
            if (heart_button.isClicked) {
                startGame(); // Reset dữ liệu rắn trước khi vào
                changeScreen(screenStack, ScreenState::InGame);
            }

			// NÚT LOAD GAME
            if (loadgame_button.isClicked)
                changeScreen(screenStack, ScreenState::LoadGame);
            break;

			// =================== IN GAME ===================
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
            if (STATE == 0 && !isLose && !isPause) {
                isLose = true;
                backgroundMusic.pause();
            }
			// 5. Xử lý lose modal
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

            // 6. Xử lý Back button
            if (back_button.isClicked) {
				isPause = !isPause;
            }

			// 7. Xử lý Pause modal
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

            // Chỉ update logic khi không pause
            if (!isPause && !isLose && !isSaving && timeAccumulator > 0.5f / (SPEED > 0 ? SPEED : 1)) {
                timeAccumulator = 0.f;
                updateGameLogic();
            }

            // 8. UI phụ, save, win...
            window.draw(timeText);

			// Save game modal
            if (isSave) {
				isSaving = true;

                backgroundMusic.pause();

                // Call modal save (this will poll events and draw popup)
                SaveGame(window, isSave);

                // After SaveGame returns: user either saved or canceled
                // If closed (isSave==false) resume game state
                if (!isSave) {
					isSaving = false;
                    // reset timing so we don't immediately run many updates
                    gameClock.restart();
                    timeAccumulator = 0.f;
                    backgroundMusic.play();
                }

                // Skip remaining InGame processing this frame (modal was shown)
                window.display();
                continue; // back to top of while(window.isOpen())
            }
            if (isWin) changeScreen(screenStack, ScreenState::MainMenu);

            break;
        }

		// =================== ABOUT US ===================
        case ScreenState::About:
            window.draw(about_us);
            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);
            break;

		// =================== LOAD GAME ===================
        case ScreenState::LoadGame:
        {
            static bool listLoaded = false;
            if (!listLoaded) {
                loadSaveFileList(saveFileInfos);
                prepareSaveFileTexts(saveFileInfos, saveFileTexts, font);
                listLoaded = true;
            }

            window.draw(load_game);
            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked) {
                goBack(screenStack);
                listLoaded = false;
            }

            // Vẽ slots và text
            for (int i = 0; i < slots.size(); i++)
            {
                updateButton(slots[i], window);
                drawButton(window, slots[i]);

                if (slots[i].isClicked && i < (int)saveFileInfos.size()) {
                    selectedSlot = i;
                    slots[i].isClicked = false;
                }

                // Hiển thị tên file trong slot
                if (i < (int)saveFileTexts.size()) {
                    saveFileTexts[i].setFillColor(
                        selectedSlot == i ? sf::Color::Yellow : sf::Color::White
                    );
                    window.draw(saveFileTexts[i]);
                }
            }

            // === HIỂN THỊ THÔNG TIN CHI TIẾT Ở BẢNG INFORMATION ===
            if (selectedSlot >= 0 && selectedSlot < (int)saveFileInfos.size()) {
                const SaveFileInfo& selected = saveFileInfos[selectedSlot];

                // NAME
                sf::Text nameText;
                nameText.setFont(font);
                nameText.setCharacterSize(22);
                nameText.setFillColor(sf::Color::Black);
                nameText.setPosition(800, 295);
                nameText.setString(selected.displayName);
                window.draw(nameText);

                // SCORE
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(22);
                scoreText.setFillColor(sf::Color::Black);
                scoreText.setPosition(800, 340);
                scoreText.setString(std::to_string(selected.score));
                window.draw(scoreText);

                // LEVEL
                sf::Text levelText;
                levelText.setFont(font);
                levelText.setCharacterSize(22);
                levelText.setFillColor(sf::Color::Black);
                levelText.setPosition(800, 385);
                levelText.setString(std::to_string(selected.level));
                window.draw(levelText);
            }

            // Rocks
            for (int i = 0; i < rocks.size(); i++) {
                updateButton(rocks[i], window);
                drawButton(window, rocks[i]);
            }

            // Rock[0]: Rename
            if (rocks[0].isClicked) {
                if (selectedSlot >= 0 && selectedSlot < (int)saveFileInfos.size()) {
                    isRename = true;
                }
                rocks[0].isClicked = false;
            }

            // Rock[1]: Load game
            if (rocks[1].isClicked) {
                if (selectedSlot >= 0 && selectedSlot < (int)saveFileInfos.size()) {
                    std::string filepath = "SaveFiles/" + saveFileInfos[selectedSlot].filename;
                    if (LoadGameFromFile(filepath)) {
                        changeScreen(screenStack, ScreenState::InGame);
                        STATE = 1;
                        listLoaded = false;
                        selectedSlot = -1;
                    }
                }
                rocks[1].isClicked = false;
            }

            // Rock[2]: Delete
            if (rocks[2].isClicked) {
                if (selectedSlot >= 0) {
                    isDelete = true;
                }
                rocks[2].isClicked = false;
            }
			// Rename modal
            if (isRename) {
                RenameDialog(window, isRename, renameOutput);

                if (!isRename && !renameOutput.empty()) {
                    // User đã confirm rename
                    if (renameSaveFile(selectedSlot, renameOutput, saveFileInfos)) {
                        prepareSaveFileTexts(saveFileInfos, saveFileTexts, font);
                        std::cout << "Renamed successfully!" << std::endl;
                    }
                    else {
                        std::cout << "Rename failed (name conflict or error)" << std::endl;
                    }
                    renameOutput = "";
                }

                window.display();
                continue;
            }
			// Delete modal
            if (isDelete) {
                deleteEffect(window, isDelete);
                if (!isDelete && selectedSlot >= 0) {
                    deleteSaveFile(selectedSlot, saveFileInfos);
                    prepareSaveFileTexts(saveFileInfos, saveFileTexts, font);
                    selectedSlot = -1;
                }
            }
            break;
        }
		// =================== HOW TO PLAY ===================
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
