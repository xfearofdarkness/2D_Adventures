//
// Created by Jamie Huta on 12.01.25.
//

#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

class Level {
public:
    Level();
    ~Level();
    static std::vector<std::vector<int>> loadTilemap(const std::string& filename);
    [[nodiscard]] std::vector<std::vector<int>> GetTileMap() const {
        return m_tilemap;
    }

    [[nodiscard]] int tile_size() const {
        return tileSize;
    }

private:
    std::vector<std::vector<int>> m_tilemap;
    int tileSize = 32;
};



#endif //LEVEL_H
