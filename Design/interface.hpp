#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "textureManager.hpp"

struct Button
{
    sf::Sprite sprite;
    sf::Texture normalTexture;
    sf::Texture hoverTexture;
    bool hasHoverTexture = false;

    bool isHovered = false;
    bool isClicked = false;
    bool isPressed = false;

    sf::Vector2f originalPos;
    sf::Vector2f hoverOffset = {0.f, 0.f};
};
// các hàm liên quan đến nút bấm giao diện
Button createButton(const std::string &normalPath, const std::string &hoverPath, float x, float y);
void updateButton(Button &btn, sf::RenderWindow &window);
void drawButton(sf::RenderWindow &window, Button &btn);
// hàm load giao diện (background)
sf::Sprite loadInterface(const std::string &path, const std::string &filename, const sf::RenderWindow &window);

#endif