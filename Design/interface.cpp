#include "interface.hpp"

// function helps load interface to fix with the fixed screen 
sf::Sprite loadInterface (const std::string& path, const std::string& filename, const sf::RenderWindow& window){
    loadTexture(path, filename);
    sf::Sprite spr(textures[filename]);
    
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = textures[filename].getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    spr.setScale(scaleX, scaleY);

    return spr;
}