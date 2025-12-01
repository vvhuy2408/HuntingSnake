#include "interface.hpp"
// ========================= BUTTON EFFECT =========================
// Function to create a button with normal and hover textures
Button createButton(const std::string &normalPath, const std::string &hoverPath, float x, float y)
{
    Button btn;
    if (!btn.normalTexture.loadFromFile(normalPath))
        std::cout << "Error loading button texture: " << normalPath << "\n";

    btn.sprite.setTexture(btn.normalTexture);
    btn.originalPos = sf::Vector2f(x, y);
    btn.sprite.setPosition(btn.originalPos);

    if (!hoverPath.empty())
    {
        if (btn.hoverTexture.loadFromFile(hoverPath))
            btn.hasHoverTexture = true;
        else
            std::cout << "Warning: failed to load hover texture: " << hoverPath << "\n";
    }


    sf::FloatRect bounds = btn.sprite.getLocalBounds();
    btn.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    btn.isHovered = false;
    btn.isClicked = false;
    btn.isPressed = false;

    return btn;
}

// Function to update the button state based on mouse interaction
void updateButton(Button &btn, sf::RenderWindow &window)
{
    btn.isClicked = false;

    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    bool mouseInside = btn.sprite.getGlobalBounds().contains((float)mouse.x, (float)mouse.y);

    btn.isHovered = mouseInside;


    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn.isHovered)
    {
        btn.isPressed = true;
    }
    else
    {
        if (btn.isPressed && btn.isHovered)
        {
            btn.isClicked = true; 
        }
        btn.isPressed = false;
    }

    if (btn.isPressed)
    {
        btn.sprite.setScale(1.15f, 1.15f);
        btn.sprite.setColor(sf::Color(255, 255, 255, 255));
    }
    else if (btn.isHovered)
    {
        btn.sprite.setScale(1.08f, 1.08f);
        btn.sprite.setColor(sf::Color(255, 255, 255, 230));

        if (btn.hasHoverTexture)
        {
            btn.sprite.setTexture(btn.hoverTexture, true);
        }
        btn.sprite.setPosition(btn.originalPos + btn.hoverOffset);
    }
    else
    {
        btn.sprite.setScale(1.0f, 1.0f);
        btn.sprite.setColor(sf::Color(255, 255, 255, 255));

        btn.sprite.setTexture(btn.normalTexture, true);
        btn.sprite.setPosition(btn.originalPos);
    }
}

// Function to draw the button on the window
void drawButton(sf::RenderWindow& window, Button& btn){
    window.draw(btn.sprite);
}

// function helps load interface to fix with the fixed screen 
sf::Sprite loadInterface(const std::string& path, const std::string& filename, const sf::RenderWindow& window){
    loadTexture(path, filename);
    sf::Sprite spr(textures[filename]);
    
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = textures[filename].getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    spr.setScale(scaleX, scaleY);

    return spr;
}