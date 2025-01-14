//
// Created by jamie on 03.01.2025.
//

#include "Enemy.h"
#include <cmath>

void Enemy::moveTowardPlayer(const Vector2& playerPosition, float deltaTime) {
    //TODO: implement enemy not following player if out of range
    float speed = 75.0f; // Enemy speed
    Vector2 directionVector = { playerPosition.x - x, playerPosition.y - y };

    if (fabs(directionVector.x) > fabs(directionVector.y)) {
        direction = (directionVector.x > 0) ? 3 : 2; // Right or Left
    } else {
        direction = (directionVector.y > 0) ? 0 : 1; // Down or Up
    }
    // Normalize the direction vector for movement
    float distance = sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);
    if (distance > 0.0f) {
        directionVector.x /= distance;
        directionVector.y /= distance;

        // Move the enemy
        float deltaX = directionVector.x * speed * deltaTime;
        float deltaY = directionVector.y * speed * deltaTime;
        x += deltaX;
        y += deltaY;

        // Update stepCount and animation
        stepCount += sqrt(deltaX * deltaX + deltaY * deltaY);
        if (stepCount > 16.0f) { // Change frame after traveling 32 pixels
            stepCount = 0.0f;
            animationIndex = (animationIndex + 1) % 2; // 2 frames per direction
        }
    }
}
