//
// Created by Jamie Huta on 12.01.25.
//

#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "Tile.h"

class Level {
public:
    Level(int lvl);

    ~Level();
    std::vector<std::vector<TileType>> loadTilemap(const std::string& filename);

    [[nodiscard]] TileType getTileAt(int x, int y) const;

    [[nodiscard]] const std::vector<std::vector<TileType>>& GetTileMap() const { return m_tilemap; }

    [[nodiscard]] int tile_size() const {
        return tileSize;
    }

    void SetTileAt(int x, int y, TileType type);

    void reload(int lvl);

    bool needsRefreshing = false;
    int getLevel() {
        return m_lvl;
    }

    void setLevel(int lvl) {
        m_lvl = lvl;
    }
private:
    int m_lvl;
    std::vector<std::vector<TileType>> m_tilemap;
    int tileSize = 32;
};



#endif //LEVEL_H
