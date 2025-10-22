#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

struct Button {
    sf::Sprite spr;
    bool isPressed = false;
    bool isClicked = false;
};

Button creatButton(const std::string& path, const std::string& name, float x, float y);
void updateButton(Button& btn, sf::RenderWindow& window);
void drawButton(sf::RenderWindow& window, Button& btn);
void exitEffect(sf::RenderWindow& window, bool& isExit);

#endif