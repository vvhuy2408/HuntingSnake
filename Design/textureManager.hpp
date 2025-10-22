#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>

extern std::map<std::string, sf::Texture> textures;
void loadTexture (const std::string& path, const std::string& name);
sf::Sprite makeSprite (const std::string& name, float x, float y);

#endif