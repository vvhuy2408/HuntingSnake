#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include "interface.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

void exitEffect(sf::RenderWindow& window, bool& isExit);
void deleteEffect(sf::RenderWindow& window, bool& isDelete);
void victoryEffect(sf::RenderWindow& window, bool& isVictory);
int loseEffect(sf::RenderWindow& window); 
void SaveGame(sf::RenderWindow& window, bool& isSave);
void PauseGame(sf::RenderWindow& window, bool& isPause);
#endif