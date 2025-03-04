//
// Created by jamie on 04.03.2025.
//

#include "EndBoss.h"

#include "EndBoss.h"
#include "raylib.h"
#include "raymath.h"

EndBoss::EndBoss(Vector2 pos, Level &level)
    : Enemy(pos, level)
{
    // Adjust EndBoss properties: slower movement and higher health.
    m_speed = 20.0f;   // EndBoss moves slower.
    lives = 20;       // High health.
    // Other properties (animationIndex, stepCount, etc.) remain as in Enemy.
}

void EndBoss::update(const Vector2 &playerPosition, float deltaTime) {
    // Use the same behavior as the base Enemy.
    Enemy::update(playerPosition, deltaTime);
    // You can add extra EndBoss-specific behavior here if needed.
}

void EndBoss::render(Texture2D &tileAtlas) {
    // Render the EndBoss with a red tint.
    // Assuming the Enemy's animation works as follows:
    //   Each frame is 32x32 in the tile atlas,
    //   and the source rectangle is computed from animationIndex and direction.
    Rectangle srcRect = { static_cast<float>(animationIndex * 32), static_cast<float>(direction * 32), 32.0f, 32.0f };
    Rectangle destRect = { pos.x, pos.y, 32.0f, 32.0f };

    // Draw the texture tinted red.
    DrawTexturePro(tileAtlas, srcRect, destRect, {0,0}, 0.0f, RED);
}