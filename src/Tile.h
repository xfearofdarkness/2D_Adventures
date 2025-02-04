//
// Created by jamie on 18.01.2025.
//

#ifndef TILE_H
#define TILE_H

enum class TileType {
    Grass,
    Water,
    Sand,
    Rock,
    Tree,
    Barrier
};

[[nodiscard]] inline bool isSolid(TileType type) {
        return type == TileType::Tree || type == TileType::Water || type == TileType::Barrier;
    };




#endif //TILE_H
