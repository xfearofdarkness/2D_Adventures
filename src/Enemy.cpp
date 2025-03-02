//
// Created by jamie on 03.01.2025.
//

#include "Enemy.h"
#include <cmath>
#include "raymath.h"
#include "Tile.h"
#include "Player.h"

Enemy::Enemy(Vector2 pos, std::vector<std::vector<TileType>>& tileMap) : pos(pos), m_tileMap(tileMap) {


}

void Enemy::setState(EnemyState state) {
    m_state = state;
}

void Enemy::update(const Vector2 &playerPosition, float deltaTime) {
    Vector2 directionVector{};
    m_stateTimer -= deltaTime;

    if (m_stateTimer <= 0.0f) {
        if (m_state == EnemyState::WANDERING) {
            m_state = EnemyState::CHASING;
            m_stateTimer = static_cast<float>(GetRandomValue(2, 5));
        } else {
            m_state = EnemyState::WANDERING;
            directionVector = {
                static_cast<float>(GetRandomValue(-1, 1)),
                static_cast<float>(GetRandomValue(-1, 1))
            };
            m_stateTimer = static_cast<float>(GetRandomValue(1, 8));
        }
    }


    if (m_state == EnemyState::WANDERING) {



        if (directionVector.x != 0 || directionVector.y != 0) {
            Vector2Normalize(directionVector);
        }

        Vector2 moveVec = {
            directionVector.x * m_speed * deltaTime,
            directionVector.y * m_speed * deltaTime
        };

        moveWithCollision(moveVec);
    } else {
        moveTowardPlayer(playerPosition, deltaTime);
    }
}

void Enemy::moveTowardPlayer(const Vector2& playerPosition, float deltaTime) {
    Vector2 enemyVec = pos;
    const float distanceToPlayer = Vector2Distance(playerPosition, enemyVec);
    if (distanceToPlayer > 200.0f) return;

    // Enemy speed
    Vector2 directionVector = { playerPosition.x - pos.x, playerPosition.y - pos.y };

    //Check the current direction
    if (fabs(directionVector.x) > fabs(directionVector.y)) {
        direction = (directionVector.x > 0) ? 3 : 2; // Right or Left
    } else {
        direction = (directionVector.y > 0) ? 0 : 1; // Down or Up
    }

    if (const float distance = Vector2Length(directionVector); distance > 0.0f) {
        directionVector.x /= distance;
        directionVector.y /= distance;

        // Move the enemy
        float deltaX = directionVector.x * m_speed * deltaTime;
        float deltaY = directionVector.y * m_speed * deltaTime;
        Vector2 moveVec = {0, 0};
        moveVec.x += deltaX;
        moveVec.y += deltaY;

        moveWithCollision(moveVec);

        // Update stepCount and animation
        stepCount += sqrt(deltaX * deltaX + deltaY * deltaY);
        if (stepCount > 16.0f) { // Change frame after traveling 32 pixels
            stepCount = 0.0f;
            animationIndex = (animationIndex + 1) % 2; // 2 frames per direction
        }
    }
}

bool Enemy::checkCollisionWithPlayer(const Rectangle &playerBoundingBox) const {
    return CheckCollisionRecs(playerBoundingBox, getBoundingBox());
}

bool Enemy::checkCollision(Vector2 testPos) {
    //adjust bounds for sprite offset
    Rectangle bounds = {
        testPos.x+4,
        testPos.y+2,
        getBoundingBox().width,
        getBoundingBox().height
    };

    int startX = static_cast<int>(std::floor(bounds.x / 32));
    int startY = static_cast<int>(std::floor(bounds.y / 32));
    int endX = static_cast<int>(std::floor((bounds.x + bounds.width) / 32));
    int endY = static_cast<int>(std::floor((bounds.y + bounds.height) / 32));

    // Check each potentially colliding tile
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            if (x < 0 || y < 0 ||
                x >= static_cast<int>(m_tileMap[0].size()) ||
                y >= static_cast<int>(m_tileMap.size())) {
                return true; // Collision with map bounds
                }

            if (isSolid(m_tileMap[y][x])) {
                Rectangle tileRect = {
                    x * 32.0f,
                    y * 32.0f,
                    32.0f,
                    32.0f
                };

                if (CheckCollisionRecs(bounds, tileRect)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Enemy::moveWithCollision(Vector2 moveVec) {

    // Try moving on X axis first
    if (moveVec.x != 0) {
        Vector2 xMovement = {moveVec.x, 0};
        if (!checkCollision({this->pos.x + xMovement.x, this->pos.y})) {
            this->pos.x += xMovement.x;
        }
    }
    // Then try moving on Y axis
    if (moveVec.y != 0) {
        Vector2 yMovement = {0, moveVec.y};
        if (!checkCollision({this->pos.x, this->pos.y + yMovement.y})) {
            this->pos.y += yMovement.y;
        }
    }
}

void Enemy::attack(Player &player, int damage) {
    if (m_state == EnemyState::WANDERING) return;
    player.takeDamage(damage);
}

void Enemy::takeDamage(int damage) {
    lives -= damage;
    //Check for death
    if (lives <= 0) {
        lives = 0;
        isAlive = false;
    }

}

