//
// Created by jamie on 03.03.2025.
//

#include "Slime.h"

#include <iostream>

#include "Player.h"
#include "raylib.h"
#include "raymath.h"

Slime::Slime(Vector2 pos, Level &level)
    : Enemy(pos, level), m_animationTimer(0.0f)
{
    m_speed = 30.0f;  // Example speed for a slime
    lives = 3;        // Low health for a slime
    // For slimes, the direction doesn't matter for animation; we use our custom timer.
    animationIndex = 0;
}

void Slime::update(Player &player, float deltaTime) {
    // Update custom animation timer.
    m_animationTimer += deltaTime;
    if (m_animationTimer >= m_animationSpeed) {
        m_animationTimer -= m_animationSpeed;
        animationIndex = (animationIndex + 1) % 3;  // Cycle through 0,1,2,3.
    }

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
            m_stateTimer = static_cast<float>(GetRandomValue(1, 4));
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
        moveTowardPlayer(player.pos, deltaTime);
    }
    // Use base behavior for movement: for instance, chase the player.
    // (You can add custom behavior if desired.)
    moveTowardPlayer(player.pos, deltaTime);
}

void Slime::render(Texture2D &tileAtlas) {
    // Define source rectangle based on our custom animation.
    // The slime frames start at (0,192) in the tile atlas, each 32x32.
    Rectangle srcRect = { animationIndex * 32.0f, 192.0f, 32.0f, 32.0f };
    // Define destination rectangle at the slime's current position.
    Rectangle destRect = { pos.x, pos.y, 32.0f, 32.0f };
    DrawTexturePro(tileAtlas, srcRect, destRect, {0,0}, 0.0f, WHITE);
}

void Slime::setState(EnemyState state) {
    m_state = state;
}

