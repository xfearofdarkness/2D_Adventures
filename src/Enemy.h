//
// Created by jamie on 03.01.2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include <vector>


#include "raylib.h"
#include "Tile.h"

enum class EnemyState {
    WANDERING,
    CHASING
};
class Player;

class Enemy {
public:
    Enemy(Vector2 position, std::vector<std::vector<TileType>>& tilemap);

    void setState(EnemyState state);

    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0;
    float animationTimer = 0.0f;
    float stepCount = 0.0f;
    Vector2 pos;
    Rectangle srcRect = { 0, 32, 32, 32 };
    int lives = 8;


    [[nodiscard]] Rectangle getBoundingBox() const {
        return { pos.x+4, pos.y+2, 24, 28 };  // Define the enemy's collision box
    }
    void update(const Vector2& playerPosition, float deltaTime);
    void moveTowardPlayer(const Vector2& playerPosition, float deltaTime);

    [[nodiscard]] bool checkCollisionWithPlayer(const Rectangle &playerBoundingBox) const;

    bool checkCollision(Vector2 testPos);
    void takeDamage(int damage);
    void moveWithCollision(Vector2 moveVec);
    void attack(Player &player, int damage);

    bool isAlive = true;

private:
    std::vector<std::vector<TileType>> m_tileMap;

    float m_speed = 75.0f;

    EnemyState m_state = EnemyState::WANDERING;

    float m_stateTimer = 0.0f;
};






#endif //ENEMY_H
