#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include "interface.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

void exitEffect(sf::RenderWindow& window, bool& isExit);
void deleteEffect(sf::RenderWindow& window, bool& isDelete);
void victoryEffect(sf::RenderWindow& window, bool& isVictory);
void loseEffect(sf::RenderWindow& window, bool& isLose);
void SaveGame(sf::RenderWindow& window, bool& isSave);

#endif