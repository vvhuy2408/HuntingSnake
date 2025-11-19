#include "textureManager.hpp"
std::map<std::string, sf::Texture> textures;

void loadTexture (const std::string& path, const std::string& name){
    if (!textures[name].loadFromFile(path))
        std::cerr << "Failed to load " << path << std::endl;
    
}

sf::Sprite makeSprite (const std::string& name, float x, float y){
    sf::Sprite spr (textures[name]);
    spr.setPosition(x, y);
    return spr;
}