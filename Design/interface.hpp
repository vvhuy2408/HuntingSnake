#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "textureManager.hpp"

sf::Sprite loadInterface(const std::string& path, const std::string& filename, const sf::RenderWindow& window);

#endif