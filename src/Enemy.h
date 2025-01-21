//
// Created by jamie on 03.01.2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "raylib.h"

struct Enemy {
    float x{}, y{};
    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0;
    float animationTimer = 0.0f;
    float stepCount = 0.0f;
    Vector2 pos{x, y};
    Rectangle srcRect = { 0, 32, 32, 32 };
    int lives = 1;
    [[nodiscard]] Rectangle getBoundingBox() const {
        return { x, y, 32, 32 }; // Define the enemy's collision box
    }
    void update(float deltaTime);
    void moveTowardPlayer(const Vector2& playerPosition, float deltaTime);

    [[nodiscard]] bool checkCollision(const Rectangle &playerBoundingBox) const;

};






#endif //ENEMY_H
