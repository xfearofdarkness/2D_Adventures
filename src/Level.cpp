//
// Created by Jamie Huta on 12.01.25.
//

#include "Level.h"

#include <iostream>

Level::Level() {
    m_tilemap = loadTilemap("res/TestMap.csv");
}

Level::~Level() {

}

std::vector<std::vector<TileType> > Level::loadTilemap(const std::string &filename) {
    std::vector<std::vector<TileType>> tilemap;
    tilemap.reserve(1024);
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + filename);
    }

    std::string line;
    int lineNum = 0;
    int rowNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        std::vector<TileType> row;
        std::stringstream ss(line);
        std::string value;
        while (std::getline(ss, value, ',')) {
            try {
                int tileValue = std::stoi(value);
                auto tileType = static_cast<TileType>(tileValue);  // Convert to TileType enum because otherwise it is implementation defined
                row.push_back(tileType);
            } catch (std::invalid_argument) {
                throw std::runtime_error("Error: Invalid value at line " + std::to_string(lineNum) + ": " + value);
            } catch (std::out_of_range) {
                throw std::runtime_error("Error: Value out of range at line: " + std::to_string(lineNum) + ": " + value);
            }

        }
        tilemap.push_back(row);
        rowNum++;
    }
    std::cout << "rows: "<< rowNum << " lines: " << lineNum << std::endl;
    std::cout << "tilemap loaded successfully" << std::endl;
    return tilemap;
}

TileType Level::getTileAt(int playerX, int playerY) const {
    // Convert player pixel position to tile indices
    int tileX = playerX / 32;
    int tileY = playerY / 32;

    // Bounds check
    if (tileX < 0 || tileX >= m_tilemap[0].size() || tileY < 0 || tileY >= m_tilemap.size()) {
        std::cout << "out of range for: " << tileX << ", " << tileY << std::endl;
        return TileType::Barrier;
    }

    return m_tilemap[tileY][tileX];
}

void Level::SetTileAt(int x, int y, TileType type) {
    int tileX = x / 32;
    int tileY = y / 32;

    if (tileX < 0 || tileX >= m_tilemap[0].size() || tileY < 0 || tileY >= m_tilemap.size()) {
        std::cout << "out of range for: " << tileX << ", " << tileY << std::endl;
        return;
    }
    m_tilemap[tileY][tileX] = type;
    needsRefreshing = true;
}

void Level::reload() {
    m_tilemap = loadTilemap("res/TestMap.csv");
}

