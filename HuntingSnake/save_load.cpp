#include "save_load.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../HuntingSnake/gameLogic.h"
#include "../HuntingSnake/globals.h"

// Trả về tên file (kèm .txt) không trùng trong thư mục dir, với tiền tố prefix.
// Ví dụ prefix="save_" -> "save_1.txt", "save_2.txt", ...
std::string getNextAvailableSaveName(const std::string& dir, const std::string& prefix) {
    namespace fs = std::filesystem;
    int idx = 1;
    // đảm bảo folder tồn tại
    if (!fs::exists(dir)) {
        fs::create_directories(dir);
    }
    while (true) {
        std::string name = prefix + std::to_string(idx) + ".txt";
        fs::path p = fs::path(dir) / name;
        if (!fs::exists(p)) return name;
        ++idx;
        // an toàn: nếu quá lớn thì break, nhưng gần như không xảy ra
        if (idx > 1000000) return prefix + std::to_string(std::rand()) + ".txt";
    }
}

void SaveGameToFile(const std::string& filename)
{
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        std::cerr << "SaveGame: cannot open " << filename << std::endl;
        return;
    }

    // Header: basic game state
    ofs << "LEVEL " << LEVEL << "\n";
    ofs << "LEVEL_PROGRESS " << LEVEL_PROGRESS << "\n";
    ofs << "LEVEL_TARGET " << LEVEL_TARGET << "\n";
    ofs << "SCORE " << SCORE << "\n";
    ofs << "SPEED " << SPEED << "\n";
    ofs << "SIZE_SNAKE " << SIZE_SNAKE << "\n";
    ofs << "STATE " << STATE << "\n";
    ofs << "MOVING " << MOVING << "\n";
    ofs << "CHAR_LOCK " << CHAR_LOCK << "\n";
    ofs << "WIDTH_CONSOLE " << WIDTH_CONSOLE << " HEIGHT_CONSOLE " << HEIGHT_CONSOLE << "\n";

    // Snake positions
    ofs << "SNAKE\n";
    for (int i = 0; i < SIZE_SNAKE; ++i) {
        ofs << snake[i].x << " " << snake[i].y << "\n";
    }

    // SNAKE_VISIBLE array
    ofs << "SNAKE_VISIBLE\n";
    for (int i = 0; i < SIZE_SNAKE; ++i) {
        ofs << (SNAKE_VISIBLE[i] ? 1 : 0) << (i + 1 < SIZE_SNAKE ? " " : "\n");
    }

    // Food
    ofs << "FOOD " << food[0].x << " " << food[0].y << "\n";
    ofs << "FOOD_TYPE " << FOOD_TYPE[0] << "\n";
    ofs << "FOOD_ACTIVE " << (FOOD_ACTIVE ? 1 : 0) << "\n";

    // Gate
    ofs << "GATE_ACTIVE " << (GATE_ACTIVE ? 1 : 0) << "\n";
    ofs << "GATE_SIZE " << GATE_SIZE << "\n";
    ofs << "GATE_POS " << GATE_POS.x << " " << GATE_POS.y << "\n";
    ofs << "GATE\n";
    for (int i = 0; i < GATE_SIZE; ++i) {
        ofs << GATE[i].x << " " << GATE[i].y << "\n";
    }

    // === THÊM PHẦN ENEMY ===
    ofs << "ENEMY_COUNT " << enemies.size() << "\n";
    ofs << "ENEMIES\n";
    for (const auto& enemy : enemies) {
        ofs << enemy.pos.x << " " << enemy.pos.y << " ";
        ofs << (int)enemy.type << " ";
        ofs << (enemy.active ? 1 : 0) << " ";
        ofs << enemy.direction << " ";
        ofs << enemy.shootTimer << "\n";
    }

    // === THÊM PHẦN BULLET ===
    ofs << "BULLET_COUNT " << bullets.size() << "\n";
    ofs << "BULLETS\n";
    for (const auto& bullet : bullets) {
        ofs << bullet.pos.x << " " << bullet.pos.y << " ";
        ofs << bullet.dx << " " << bullet.dy << " ";
        ofs << (bullet.active ? 1 : 0) << "\n";
    }

    ofs.close();
    std::cout << "Game saved to " << filename << std::endl;
}

bool LoadGameFromFile(const std::string& filename)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "LoadGame: cannot open " << filename << std::endl;
        return false;
    }

    // Xóa dữ liệu enemy/bullet cũ trước khi load
    enemies.clear();
    bullets.clear();

    int enemyCount = 0;
    int bulletCount = 0;

    std::string tag;
    while (ifs >> tag) {
        if (tag == "LEVEL") { ifs >> LEVEL; }
        else if (tag == "LEVEL_PROGRESS") { ifs >> LEVEL_PROGRESS; }
        else if (tag == "LEVEL_TARGET") { ifs >> LEVEL_TARGET; }
        else if (tag == "SCORE") { ifs >> SCORE; }
        else if (tag == "SPEED") { ifs >> SPEED; }
        else if (tag == "SIZE_SNAKE") { ifs >> SIZE_SNAKE; }
        else if (tag == "STATE") { ifs >> STATE; }
        else if (tag == "MOVING") { ifs >> MOVING; }
        else if (tag == "CHAR_LOCK") { ifs >> CHAR_LOCK; }
        else if (tag == "WIDTH_CONSOLE") {
            ifs >> WIDTH_CONSOLE;
            ifs >> tag;
            if (tag == "HEIGHT_CONSOLE") ifs >> HEIGHT_CONSOLE;
        }
        else if (tag == "SNAKE") {
            for (int i = 0; i < SIZE_SNAKE; ++i) {
                ifs >> snake[i].x >> snake[i].y;
            }
        }
        else if (tag == "SNAKE_VISIBLE") {
            for (int i = 0; i < SIZE_SNAKE; ++i) {
                int v; ifs >> v; SNAKE_VISIBLE[i] = (v != 0);
            }
        }
        else if (tag == "FOOD") {
            ifs >> food[0].x >> food[0].y;
        }
        else if (tag == "FOOD_TYPE") {
            ifs >> FOOD_TYPE[0];
        }
        else if (tag == "FOOD_ACTIVE") {
            int v; ifs >> v; FOOD_ACTIVE = (v != 0);
        }
        else if (tag == "GATE_ACTIVE") {
            int v; ifs >> v; GATE_ACTIVE = (v != 0);
        }
        else if (tag == "GATE_SIZE") {
            ifs >> GATE_SIZE;
        }
        else if (tag == "GATE_POS") {
            ifs >> GATE_POS.x >> GATE_POS.y;
        }
        else if (tag == "GATE") {
            for (int i = 0; i < GATE_SIZE; ++i) {
                ifs >> GATE[i].x >> GATE[i].y;
            }
        }
        // === THÊM PHẦN ĐỌC ENEMY ===
        else if (tag == "ENEMY_COUNT") {
            ifs >> enemyCount;
        }
        else if (tag == "ENEMIES") {
            for (int i = 0; i < enemyCount; ++i) {
                Enemy enemy;
                int type, active;
                ifs >> enemy.pos.x >> enemy.pos.y;
                ifs >> type;
                ifs >> active;
                ifs >> enemy.direction;
                ifs >> enemy.shootTimer;

                enemy.type = (EnemyType)type;
                enemy.active = (active != 0);

                enemies.push_back(enemy);
            }
        }
        // === THÊM PHẦN ĐỌC BULLET ===
        else if (tag == "BULLET_COUNT") {
            ifs >> bulletCount;
        }
        else if (tag == "BULLETS") {
            for (int i = 0; i < bulletCount; ++i) {
                Bullet bullet;
                int active;
                ifs >> bullet.pos.x >> bullet.pos.y;
                ifs >> bullet.dx >> bullet.dy;
                ifs >> active;

                bullet.active = (active != 0);

                bullets.push_back(bullet);
            }
        }
        else {
            // unknown token: skip line
            std::string rest;
            std::getline(ifs, rest);
        }
    }

    ifs.close();

    // After loading, rebuild walls for the loaded level
    buildWalls(LEVEL);

    // Ensure snake visible flags for indices beyond SIZE_SNAKE are false
    for (int i = SIZE_SNAKE; i < MAX_SIZE_SNAKE; ++i) {
        SNAKE_VISIBLE[i] = false;
    }

    std::cout << "Game loaded from " << filename << std::endl;
    std::cout << "Loaded " << enemies.size() << " enemies and "
        << bullets.size() << " bullets" << std::endl;

    return true;
}

// === HÀM ĐỌC THÔNG TIN TỪ FILE SAVE ===
SaveFileInfo readSaveFileInfo(const std::string& filepath) {
    SaveFileInfo info;
    namespace fs = std::filesystem;

    // Lấy tên file
    info.filename = fs::path(filepath).filename().string();

    // Tên hiển thị (bỏ .txt)
    info.displayName = info.filename;
    if (info.displayName.size() > 4 && info.displayName.substr(info.displayName.size() - 4) == ".txt") {
        info.displayName = info.displayName.substr(0, info.displayName.size() - 4);
    }

    // Đọc level và score từ file
    info.level = 1;
    info.score = 0;

    std::ifstream ifs(filepath);
    if (ifs.is_open()) {
        std::string tag;
        while (ifs >> tag) {
            if (tag == "LEVEL") {
                ifs >> info.level;
            }
            else if (tag == "SCORE") {
                ifs >> info.score;
                break; // đã đủ thông tin cần thiết
            }
        }
        ifs.close();
    }

    return info;
}

// Load danh sách file save từ save_list.txt
void loadSaveFileList(std::vector<SaveFileInfo>& saveFileInfos) {
    saveFileInfos.clear();
    const std::string saveDir = "SaveFiles";
    const std::string listFile = saveDir + "/save_list.txt";

    std::ifstream ifs(listFile);
    if (ifs.is_open()) {
        std::string filename;
        while (std::getline(ifs, filename)) {
            if (!filename.empty()) {
                std::string filepath = saveDir + "/" + filename;
                SaveFileInfo info = readSaveFileInfo(filepath);
                saveFileInfos.push_back(info);
            }
        }
        ifs.close();
    }
}

// Chuẩn bị sf::Text để hiển thị tên file
void prepareSaveFileTexts(const std::vector<SaveFileInfo>& saveFileInfos,
    std::vector<sf::Text>& saveFileTexts,
    sf::Font& font) {
    saveFileTexts.clear();
    float x_text = 125, y_text = 225;

    for (size_t i = 0; i < saveFileInfos.size() && i < 6; i++) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);

        // Lấy tên file không có .txt
        std::string displayName = saveFileInfos[i].displayName;
        if (displayName.length() > 12) {
            displayName = displayName.substr(0, 12) + "...";
        }

        text.setString(displayName);

        // Tính toán vị trí (layout 2 cột, 3 hàng)
        if (i % 2 != 0) {
            x_text += 330;
            text.setPosition(x_text, y_text);
            x_text -= 330;
        }
        else {
            if (i != 0) y_text += 150;
            text.setPosition(x_text, y_text);
        }

        saveFileTexts.push_back(text);
    }
}

// Thêm tên file mới vào save_list.txt (gọi từ SaveGame)
void updateSaveList(const std::string& newFileName) {
    const std::string saveDir = "SaveFiles";
    namespace fs = std::filesystem;

    if (!fs::exists(saveDir)) {
        fs::create_directories(saveDir);
    }

    // Đọc danh sách hiện tại
    std::vector<std::string> currentList;
    std::string listFile = saveDir + "/save_list.txt";

    std::ifstream ifs(listFile);
    if (ifs.is_open()) {
        std::string filename;
        while (std::getline(ifs, filename)) {
            if (!filename.empty()) {
                currentList.push_back(filename);
            }
        }
        ifs.close();
    }

    // Nếu đã đủ 6 slot, xóa file cũ nhất (slot đầu tiên)
    if (currentList.size() >= 6) {
        std::string oldestFile = currentList[0];
        std::string oldestPath = saveDir + "/" + oldestFile;

        // Xóa file cũ nhất
        if (fs::exists(oldestPath)) {
            fs::remove(oldestPath);
            std::cout << "Removed oldest save: " << oldestFile << std::endl;
        }

        // Xóa khỏi list
        currentList.erase(currentList.begin());
    }

    // Thêm file mới vào cuối
    currentList.push_back(newFileName);

    // Ghi lại toàn bộ danh sách
    std::ofstream ofs(listFile);
    if (ofs.is_open()) {
        for (const auto& name : currentList) {
            ofs << name << "\n";
        }
        ofs.close();
    }
}

// Xóa file save và cập nhật lại save_list.txt
void deleteSaveFile(int selectedSlot, std::vector<SaveFileInfo>& saveFileInfos) {
    namespace fs = std::filesystem;

    if (selectedSlot < 0 || selectedSlot >= (int)saveFileInfos.size()) {
        return;
    }

    const std::string saveDir = "SaveFiles";

    // Xóa file save
    std::string filepath = saveDir + "/" + saveFileInfos[selectedSlot].filename;
    if (fs::exists(filepath)) {
        fs::remove(filepath);
    }
    
    // Cập nhật lại save_list.txt
    std::ofstream ofs(saveDir + "/save_list.txt");
    if (ofs.is_open()) {
        for (size_t i = 0; i < saveFileInfos.size(); i++) {
            if ((int)i != selectedSlot) {
                ofs << saveFileInfos[i].filename << "\n";
            }
        }
        ofs.close();
    }
    
    // Xóa khỏi vector
    saveFileInfos.erase(saveFileInfos.begin() + selectedSlot);
}

bool renameSaveFile(int selectedSlot, const std::string& newName,
    std::vector<SaveFileInfo>& saveFileInfos) {
    namespace fs = std::filesystem;

    if (selectedSlot < 0 || selectedSlot >= (int)saveFileInfos.size()) {
        return false;
    }

    const std::string saveDir = "SaveFiles";
    std::string oldFilepath = saveDir + "/" + saveFileInfos[selectedSlot].filename;

    // Tạo tên file mới
    std::string newFileName = newName;
    if (newFileName.size() < 4 || newFileName.substr(newFileName.size() - 4) != ".txt") {
        newFileName += ".txt";
    }

    std::string newFilepath = saveDir + "/" + newFileName;

    // Kiểm tra trùng tên
    if (fs::exists(newFilepath) && oldFilepath != newFilepath) {
        return false; // tên đã tồn tại
    }

    // Rename file
    try {
        fs::rename(oldFilepath, newFilepath);
    }
    catch (const std::exception& e) {
        std::cerr << "Rename error: " << e.what() << std::endl;
        return false;
    }

    // Cập nhật save_list.txt
    std::ofstream ofs(saveDir + "/save_list.txt");
    if (ofs.is_open()) {
        for (size_t i = 0; i < saveFileInfos.size(); i++) {
            if ((int)i == selectedSlot) {
                ofs << newFileName << "\n";
            }
            else {
                ofs << saveFileInfos[i].filename << "\n";
            }
        }
        ofs.close();
    }

    // Cập nhật vector
    saveFileInfos[selectedSlot].filename = newFileName;
    saveFileInfos[selectedSlot].displayName = newName;

    return true;
}