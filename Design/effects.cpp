#include "effects.hpp"
#include "textureManager.hpp"
#include "../HuntingSnake/globals.h" 
#include "../HuntingSnake/save_load.h"
#include <fstream>
void exitEffect(sf::RenderWindow& window, bool& isExit){
    loadTexture("Design/Assets/exitbox.png", "exit-box");
    sf::Sprite exit_box = makeSprite("exit-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);
    
    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    window.draw(exit_box);
    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    if (confirm_button.isClicked){
        window.close();
    } else if (exit_button.isClicked){
        isExit = false;
    }
}

void deleteEffect(sf::RenderWindow& window, bool& isDelete)
{
    loadTexture("Design/Assets/deleteframe.png", "delete-box");
    sf::Sprite delete_box = makeSprite("delete-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    window.draw(delete_box);
    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    if (confirm_button.isClicked) {
        // xử lý logic
        isDelete = false;
    }
    else if (exit_button.isClicked) {
        isDelete = false;
    }
}

void victoryEffect(sf::RenderWindow& window, bool& isVictory)
{
    loadTexture("Design/Assets/victory.png", "victory-box");
    sf::Sprite victory = makeSprite("victory-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    window.draw(victory);
    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    if (confirm_button.isClicked) {
        // xử li logic
        isVictory = false;
    }
    else if (exit_button.isClicked) {
        isVictory = false;
    }
}

int loseEffect(sf::RenderWindow& window)
{
    loadTexture("Design/Assets/lose.png", "lose-box");
    sf::Sprite lose = makeSprite("lose-box", 267, 170);

    // Các button giữ trạng thái static để không recreate mọi frame
    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    // Cập nhật trạng thái button (bắt event mouse/hover/click)
    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    // Vẽ
    window.draw(lose);
    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    if (confirm_button.isClicked) {
        // Trả về confirm action
        return 1;
    }
    else if (exit_button.isClicked) {
        return 2;
    }

    return 0;
}

void SaveGame(sf::RenderWindow& window, bool& isSave)
{
    using namespace std;
    namespace fs = std::filesystem;
    const std::string saveDir = "SaveFiles";

    // assets UI
    loadTexture("Design/Assets/savegame.png", "save-box");
    sf::Sprite save = makeSprite("save-box", 267, 170);

    Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    std::string saveName = "";    // user input (without .txt)
    bool nameConflict = false;

    // Input box visual
    sf::RectangleShape inputBox(sf::Vector2f(350, 40));
    inputBox.setPosition(460, 340);
    inputBox.setFillColor(sf::Color(240, 240, 240));
    inputBox.setOutlineThickness(1);
    inputBox.setOutlineColor(sf::Color::Black);

    sf::Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition(470, 345);

    sf::Text warnText;
    warnText.setFont(font);
    warnText.setCharacterSize(20);
    warnText.setFillColor(sf::Color::Red);
    warnText.setPosition(600, 345);

    sf::Text hintText;
    hintText.setFont(font);
    hintText.setCharacterSize(20);
    hintText.setFillColor(sf::Color::White);
    hintText.setPosition(460, 315);
    hintText.setString("Input save name (max 7 chars): ");

    // === MODAL LOOP: Chạy cho đến khi user confirm hoặc exit ===
    bool modalActive = true;
    while (modalActive && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                isSave = false;
                return;
            }
            if (event.type == sf::Event::TextEntered) {
                unsigned int c = event.text.unicode;
                if (c == 8) { // backspace
                    if (!saveName.empty()) {
                        saveName.pop_back();
                        nameConflict = false; // reset warning khi sửa
                    }
                }
                else if (c == 13 || c == 10) {
                    // Enter key: trigger confirm
                    // (sẽ xử lý ở phần confirm button bên dưới)
                }
                else if (c >= 32 && c <= 126) {
                    // Allow alnum and some symbols; limit size to 7 characters
                    if (saveName.size() < 7) {
                        char ch = static_cast<char>(c);
                        // allow letters, digits, underscore, dash, space
                        if (isalnum((unsigned char)ch) || ch == '_' || ch == '-' || ch == ' ') {
                            saveName.push_back(ch);
                            nameConflict = false; // reset warning khi nhập
                        }
                    }
                }
            }
            // Escape to cancel
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                isSave = false;
                return;
            }
        }

        // Update UI buttons
        updateButton(confirm_button, window);
        updateButton(exit_button, window);

        // Vẽ lại toàn bộ modal
        window.clear();
        
        // Vẽ lại background game (nếu cần) - hoặc để đen
        // Có thể vẽ lại màn hình game phía sau làm nền mờ
        
        window.draw(save);
        window.draw(inputBox);

        nameText.setString(saveName.empty() ? "" : saveName);
        window.draw(nameText);
        window.draw(hintText);

        if (nameConflict) {
            warnText.setString("Name existed!");
            window.draw(warnText);
        }

        drawButton(window, confirm_button);
        drawButton(window, exit_button);

        window.display();

        // Handle confirm: save
        if (confirm_button.isClicked) {
            confirm_button.isClicked = false;
            
            // Ensure save directory exists
            if (!fs::exists(saveDir)) fs::create_directories(saveDir);

            std::string finalName;
            if (saveName.empty()) {
                // auto generate non-conflicting name
                finalName = getNextAvailableSaveName(saveDir, "save_");
            }
            else {
                // add .txt if user didn't
                finalName = saveName;
                if (finalName.size() < 4 || finalName.substr(finalName.size() - 4) != ".txt")
                    finalName += ".txt";
                // check conflict
                fs::path p = fs::path(saveDir) / finalName;
                if (fs::exists(p)) {
                    nameConflict = true;
                    continue; // don't close modal, show warning
                }
            }

            // Compose full path and write
            fs::path filepath = fs::path(saveDir) / finalName;
            SaveGameToFile(filepath.string());

            updateSaveList(finalName);

            std::cout << "Saved as " << filepath.string() << std::endl;
            
            isSave = false;
            modalActive = false; // Close modal
        }

        // Handle exit: close popup without saving
        if (exit_button.isClicked) {
            exit_button.isClicked = false;
            isSave = false;
            modalActive = false; // Close modal
        }
    }
}


void RenameDialog(sf::RenderWindow& window, bool& isRename, std::string& outputName)
{
    using namespace std;
    namespace fs = std::filesystem;
    const string saveDir = "SaveFiles";

    loadTexture("Design/Assets/savegame.png", "rename-box");
    sf::Sprite renameBox = makeSprite("rename-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    string newName = "";
    bool nameConflict = false;
    string warningMessage = ""; // dùng để hiển thị mọi cảnh báo

    // Input box visual
    sf::RectangleShape inputBox(sf::Vector2f(350, 40));
    inputBox.setPosition(460, 340);
    inputBox.setFillColor(sf::Color(240, 240, 240));
    inputBox.setOutlineThickness(1);
    inputBox.setOutlineColor(sf::Color::Black);

    sf::Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition(470, 345);

    sf::Text warnText;
    warnText.setFont(font);
    warnText.setCharacterSize(18);
    warnText.setFillColor(sf::Color::Red);
    warnText.setPosition(600, 345);

    sf::Text hintText;
    hintText.setFont(font);
    hintText.setCharacterSize(20);
    hintText.setFillColor(sf::Color::White);
    hintText.setPosition(460, 315);
    hintText.setString("Input new name (max 7 chars): ");

    if (!fs::exists(saveDir)) {
        fs::create_directories(saveDir);
    }

    bool modalActive = true;
    while (modalActive && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                isRename = false;
                outputName = "";
                return;
            }

            if (event.type == sf::Event::TextEntered) {
                unsigned int c = event.text.unicode;
                if (c == 8) { // backspace
                    if (!newName.empty()) {
                        newName.pop_back();
                    }
                    nameConflict = false;
                    warningMessage.clear();
                }
                else if (c == 13 || c == 10) {
                    // ignore, we'll handle confirm button
                }
                else if (c >= 32 && c <= 126) {
                    if (newName.size() < 7) {
                        char ch = static_cast<char>(c);
                        if (isalnum((unsigned char)ch) || ch == '_' || ch == '-' || ch == ' ') {
                            newName.push_back(ch);
                        }
                    }
                    nameConflict = false;
                    warningMessage.clear();
                }

                // After any change, check conflict immediately
                if (!newName.empty()) {
                    string candidate = newName;
                    if (candidate.size() < 4 || candidate.substr(candidate.size() - 4) != ".txt")
                        candidate += ".txt";
                    fs::path p = fs::path(saveDir) / candidate;
                    nameConflict = fs::exists(p);
                    if (nameConflict) warningMessage = "Name already exists!";
                    else warningMessage.clear();
                }
                else {
                    nameConflict = false;
                    // keep warningMessage as-is (so confirm can set "empty" message)
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                isRename = false;
                outputName.clear();
                return;
            }
        }

        // Update UI buttons
        updateButton(confirm_button, window);
        updateButton(exit_button, window);

        // Vẽ modal
        window.clear();
        window.draw(renameBox);
        window.draw(inputBox);

        nameText.setString(newName);
        window.draw(nameText);
        window.draw(hintText);

        // Vẽ cảnh báo nếu có
        if (!warningMessage.empty()) {
            warnText.setString(warningMessage);
            warnText.setFillColor(sf::Color::Red);
            window.draw(warnText);
        }

        drawButton(window, confirm_button);
        drawButton(window, exit_button);
        window.display();

        // Handle confirm
        if (confirm_button.isClicked) {
            confirm_button.isClicked = false;

            // Build candidate filename
            string candidate = newName;
            if (candidate.size() < 4 || candidate.substr(candidate.size() - 4) != ".txt")
                candidate += ".txt";

            if (newName.empty()) {
                // show the proper message and keep modal open
                warningMessage = "Name cannot be empty";
                // don't block UI with sleep; just continue and let render show message
                continue;
            }

            fs::path p = fs::path(saveDir) / candidate;
            if (fs::exists(p)) {
                nameConflict = true;
                warningMessage = "Name already exists!";
                continue;
            }

            // OK: accept
            outputName = newName; // note: no ".txt"; caller can append if needed
            isRename = false;
            modalActive = false;
            break;
        }

        // Handle exit
        if (exit_button.isClicked) {
            exit_button.isClicked = false;
            outputName.clear(); // empty = canceled
            isRename = false;
            modalActive = false;
            break;
        }
    } // end modal
}

void PauseGame(sf::RenderWindow& window, bool& isPause)
{
    loadTexture("Design/Assets/pause.png", "pause-box");
    sf::Sprite pause = makeSprite("pause-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 630, 427);

    updateButton(confirm_button, window);

    window.draw(pause);
    drawButton(window, confirm_button);

    if (confirm_button.isClicked) {
        // logic
        isPause = false;
		STATE = 1; // resume game
        confirm_button.isClicked = false;
    }
}

// ========================= SOUND EFFECT =========================
