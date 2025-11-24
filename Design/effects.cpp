#include "effects.hpp"
#include "textureManager.hpp"

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

    if (confirm_button.isClicked){
        // xử lý logic
        isDelete = false;
    } else if (exit_button.isClicked){
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

    if (confirm_button.isClicked){
        // xử li logic
        isVictory = false;
    } else if (exit_button.isClicked){
        isVictory = false;
    }
}

void loseEffect(sf::RenderWindow& window, bool& isLose)
{
    loadTexture("Design/Assets/lose.png", "lose-box");
    sf::Sprite lose = makeSprite("lose-box", 267, 170);

    static Button confirm_button = createButton("Design/Assets/button/confirm.png", "", 530, 427);
    static Button exit_button = createButton("Design/Assets/button/exit.png", "", 740, 427);
    
    updateButton(confirm_button, window);
    updateButton(exit_button, window);

    window.draw(lose);
    drawButton(window, confirm_button);
    drawButton(window, exit_button);

    if (confirm_button.isClicked){
        // xử lý logic
        isLose = false;
    } else if (exit_button.isClicked){
        isLose = false;
    }
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

    if (confirm_button.isClicked){
        // logic
        isSave = false;
    } else if (exit_button.isClicked){
        isSave = false;
    }
}

// ========================= SOUND EFFECT =========================
