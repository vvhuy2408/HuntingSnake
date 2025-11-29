#include "effects.hpp"
#include "textureManager.hpp"
#include "../HuntingSnake/globals.h" 

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
    loadTexture("Design/Assets/savegame.png", "save-box");
    sf::Sprite save = makeSprite("save-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);

    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    window.draw(save);
    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    if (confirm_button.isClicked) {
        // logic
        isSave = false;
    }
    else if (exit_button.isClicked) {
        isSave = false;
    }
}

void PauseGame(sf::RenderWindow& window, bool& isPause)
{
    loadTexture("Design/Assets/pause.png", "pause-box");
    sf::Sprite pause = makeSprite("pause-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 630, 427);

    updateButton(confirm_button, window);

    //window.draw(pause);
    drawButton(window, confirm_button);

    if (confirm_button.isClicked) {
        // logic
        isPause = false;
		STATE = 1; // resume game
        confirm_button.isClicked = false;
    }
}

// ========================= SOUND EFFECT =========================
