#include "effects.hpp"
#include "textureManager.hpp"
//-----------BUTTON EFFECT-----------

Button creatButton(const std::string &path, const std::string &name, float x, float y)
{
    Button btn;
    loadTexture(path, name);
    btn.spr = makeSprite(name, x, y);

    sf::FloatRect bounds = btn.spr.getLocalBounds();
    btn.spr.setOrigin(bounds.width / 2, bounds.height / 2);
    btn.spr.setScale(1.0f, 1.0f);
    btn.isPressed = false;
    btn.isClicked = false;

    return btn;
}

void updateButton(Button &btn, sf::RenderWindow &window)
{
    btn.isClicked = false;

    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    bool isHovered = btn.spr.getGlobalBounds().contains(mouse.x, mouse.y);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isHovered)
    {
        btn.isPressed = true;
    }
    else
    {
        if (btn.isPressed && isHovered)
        {
            btn.isClicked = true;
        }
        btn.isPressed = false;
    }

    if (btn.isPressed)
    {
        btn.spr.setScale(1.15f, 1.15f);
        btn.spr.setColor(sf::Color(255, 255, 255, 255));
    }
    else if (isHovered)
    {
        btn.spr.setScale(1.08f, 1.08f);
        btn.spr.setColor(sf::Color(255, 255, 255, 230));
    }
    else
    {
        btn.spr.setScale(1.0f, 1.0f);
        btn.spr.setColor(sf::Color(200, 200, 200, 220));
    }
}

void drawButton(sf::RenderWindow& window, Button& btn){
    window.draw(btn.spr);
}

void exitEffect(sf::RenderWindow& window, bool& isExit){
    loadTexture("Design/Assets/exitbox.png", "exit-box");
    sf::Sprite exit_box = makeSprite("exit-box", 267, 170);

    static Button confirm_button = creatButton("Design/Assets/button/confirm.png", "confirm-button", 530, 427);
    static Button exit_button = creatButton("Design/Assets/button/exit.png", "exit-button", 740, 427);
    
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

//----------SOUND EFFECT----------
