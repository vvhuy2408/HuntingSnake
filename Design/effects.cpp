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

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    static std::string saveName = "";    // user input (without .txt)
    static bool nameConflict = false;
    static bool showSavedMsg = false;
    static float savedMsgTimer = 0.f;

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
    hintText.setString("Input save name: ");

    // === Poll events while in modal (so modal "owns" events) ===
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (event.type == sf::Event::TextEntered) {
            unsigned int c = event.text.unicode;
            if (c == 8) { // backspace
                if (!saveName.empty()) saveName.pop_back();
            }
            else if (c == 13 || c == 10) {
                // ignore enter here (use confirm button)
            }
            else if (c >= 32 && c <= 126) {
                // Allow alnum and some symbols; limit size
                if (saveName.size() < 24) {
                    char ch = static_cast<char>(c);
                    // allow letters, digits, underscore, dash, space
                    if (isalnum((unsigned char)ch) || ch == '_' || ch == '-' || ch == ' ')
                        saveName.push_back(ch);
                }
            }
        }
        // You can also handle keypress Escape to cancel
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            saveName.clear();
            nameConflict = false;
            isSave = false;
            return;
        }
    }

    // Update UI buttons
    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    // Draw popup
    window.draw(save);
    window.draw(inputBox);

    nameText.setString(saveName.empty() ? "Empty" : saveName);
    window.draw(nameText);
    window.draw(hintText);

    if (nameConflict) {
        warnText.setString("Save name existed!");
        window.draw(warnText);
    }

    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    // Handle confirm: save
    if (confirm_button.isClicked) {
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
                confirm_button.isClicked = false;
                return; // don't close popup
            }
        }

        // Compose full path and write
        fs::path filepath = fs::path(saveDir) / finalName;
        SaveGameToFile(filepath.string());

        updateSaveList(finalName);

        // feedback: show saved message briefly (optional)
        showSavedMsg = true;
        savedMsgTimer = 2.0f; // seconds

        // reset flags and close popup
        saveName.clear();
        nameConflict = false;
        confirm_button.isClicked = false;
        isSave = false;

        std::cout << "Saved as " << filepath.string() << std::endl;
    }

    // Handle exit: close popup without saving
    if (exit_button.isClicked) {
        exit_button.isClicked = false;
        saveName.clear();
        nameConflict = false;
        isSave = false;
    }

    // Optional: show a small "Saved!" message (draw over popup) if needed.
    if (showSavedMsg) {
        // decrease timer (note: we don't have dt here; a simple approach is to poll clock or ignore)
        // For simplicity, just draw a message once (or you can manage a timer outside)
        sf::Text okText;
        okText.setFont(font);
        okText.setCharacterSize(20);
        okText.setFillColor(sf::Color::Green);
        okText.setPosition(600, 345);
        okText.setString("Saved!");
        window.draw(okText);
        // Not implementing timed fade here to keep code short.
    }
}

void RenameDialog(sf::RenderWindow& window, bool& isRename, std::string& outputName)
{
    // Assets UI (giống SaveGame)
    loadTexture("Design/Assets/savegame.png", "rename-box");
    sf::Sprite renameBox = makeSprite("rename-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    static std::string newName = "";
    static bool nameConflict = false;

    // Input box visual
    sf::RectangleShape inputBox(sf::Vector2f(350, 40));
    inputBox.setPosition(430, 340);
    inputBox.setFillColor(sf::Color(240, 240, 240));
    inputBox.setOutlineThickness(1);
    inputBox.setOutlineColor(sf::Color::Black);

    sf::Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition(440, 345);

    sf::Text warnText;
    warnText.setFont(font);
    warnText.setCharacterSize(18);
    warnText.setFillColor(sf::Color::Red);
    warnText.setPosition(435, 390);

    sf::Text hintText;
    hintText.setFont(font);
    hintText.setCharacterSize(20);
    hintText.setFillColor(sf::Color::White);
    hintText.setPosition(430, 315);
    hintText.setString("Input new name: ");

    // Poll events
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (event.type == sf::Event::TextEntered) {
            unsigned int c = event.text.unicode;
            if (c == 8) { // backspace
                if (!newName.empty()) newName.pop_back();
            }
            else if (c >= 32 && c <= 126) {
                if (newName.size() < 24) {
                    char ch = static_cast<char>(c);
                    if (isalnum((unsigned char)ch) || ch == '_' || ch == '-' || ch == ' ')
                        newName.push_back(ch);
                }
            }
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            newName.clear();
            nameConflict = false;
            isRename = false;
            return;
        }
    }

    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    // Draw popup
    window.draw(renameBox);
    window.draw(inputBox);

    nameText.setString(newName.empty() ? "Empty" : newName);
    window.draw(nameText);
    window.draw(hintText);

    if (nameConflict) {
        warnText.setString("Name already exists!");
        window.draw(warnText);
    }

    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    // Handle confirm
    if (confirm_button.isClicked) {
        if (!newName.empty()) {
            outputName = newName;
            newName.clear();
            nameConflict = false;
            confirm_button.isClicked = false;
            isRename = false;
        }
    }

    // Handle exit
    if (exit_button.isClicked) {
        exit_button.isClicked = false;
        newName.clear();
        nameConflict = false;
        outputName = ""; // empty = canceled
        isRename = false;
    }
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
