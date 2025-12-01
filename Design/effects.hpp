#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include "interface.hpp"
#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>
// Hàm hiển thị hộp thoại thoát, thua game
void exitEffect(sf::RenderWindow& window, bool& isExit);
void victoryEffect(sf::RenderWindow& window, bool& isVictory);
int loseEffect(sf::RenderWindow& window);

// Hàm hiển thị hộp thoại lưu game, xóa save, đổi tên save
void deleteEffect(sf::RenderWindow& window, bool& isDelete);
void SaveGame(sf::RenderWindow& window, bool& isSave);
void RenameDialog(sf::RenderWindow& window, bool& isRename, std::string& outputName);
void PauseGame(sf::RenderWindow& window, bool& isPause);
#endif