//
// Created by jamie on 03.01.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct Player {
    float x{}, y{};
    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0; // Animation frame index
    float animationTimer = 0.0f; // Tracks time for animation updates
    float stepCount = 0.0f; // Tracks distance traveled for animation purposes

    Rectangle srcRect = { 0, 32, 32, 32 };

    [[nodiscard]] Rectangle getBoundingBox() const {
        return { x, y, 32, 32 }; // Define the player's collision box
    }
    void update(Player& player, float deltaTime);
};

void move(Player& player, float deltaTime);

#endif // PLAYER_H

