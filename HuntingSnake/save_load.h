#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Struct lưu thông tin metadata của save file
struct SaveFileInfo {
    std::string filename;
    int level;
    int score;
    std::string displayName; // tên hiển thị (không có .txt)
};

std::string getNextAvailableSaveName(const std::string& dir, const std::string& prefix);
void SaveGameToFile(const std::string& filename);
bool LoadGameFromFile(const std::string& filename);

// Hàm mới cho load game UI
void loadSaveFileList(std::vector<SaveFileInfo>& saveFileInfos);
void prepareSaveFileTexts(const std::vector<SaveFileInfo>& saveFileInfos,
    std::vector<sf::Text>& saveFileTexts,
    sf::Font& font);
void updateSaveList(const std::string& newFileName);
void deleteSaveFile(int selectedSlot, std::vector<SaveFileInfo>& saveFileInfos);
SaveFileInfo readSaveFileInfo(const std::string& filepath);

bool renameSaveFile(int selectedSlot, const std::string& newName,
    std::vector<SaveFileInfo>& saveFileInfos);
// Wrapper phù hợp main
void SaveGame(sf::RenderWindow& window, bool& isSave); // bạn gọi trong main