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
    Level();
    ~Level();
    static std::vector<std::vector<TileType>> loadTilemap(const std::string& filename);

    TileType getTileAt(int x, int y) const;

    [[nodiscard]] std::vector<std::vector<TileType>> GetTileMap() const {
        return m_tilemap;
    }

    [[nodiscard]] int tile_size() const {
        return tileSize;
    }

    void SetTileAt(int x, int y, TileType type);

    void reload();

    bool needsRefreshing = false;
private:
    std::vector<std::vector<TileType>> m_tilemap;
    int tileSize = 32;
};



#endif //LEVEL_H
