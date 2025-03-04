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
    Stone,
    Dirt,
    LevelTransition,
    Barrier,
};

[[nodiscard]] inline bool isSolid(TileType type) {
        return type == TileType::Tree || type == TileType::Water || type == TileType::Barrier || type == TileType::Stone;
    };




#endif //TILE_H
