//
// Created by jamie on 03.01.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>


#include "raylib.h"
#include "Tile.h"
#include "Enemy.h"
class Enemy;

class Player {
    public:
    Player(Vector2 pos, const std::vector<std::vector<TileType>>& tilemap);
    ~Player();
    Vector2 pos;
    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0; // Animation frame index
    float animationTimer = 0.0f; // Tracks time for animation updates
    float stepCount = 0.0f; // Tracks distance traveled for animation purposes

    Rectangle srcRect = { 0, 32, 32, 32 };

    [[nodiscard]] Rectangle getBoundingBox() const {
        return { pos.x+4, pos.y+2, 24, 28 }; // Define the player's collision box
    }

    void update(float deltaTime, std::vector<Enemy>& enemies);
    void takeDamage(int damage);
    void attack(std::vector<Enemy>& enemies);
    Rectangle attackBoxRec;
private:
    void move(float deltaTime);

    void moveWithCollision(Vector2 moveVec);

    bool checkCollision(Vector2 testPos);

private:
    std::vector<std::vector<TileType>> m_tilemap;
    int health = 8;
    bool isAlive = true;
};





#endif // PLAYER_H

