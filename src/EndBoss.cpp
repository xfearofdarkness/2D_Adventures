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

    m_speed = 35.0f;
    lives = 20;
    m_damage = 2;
    isAlive = true;
}

void EndBoss::update(Player &player, float deltaTime) {

    Enemy::update(player, deltaTime);

}

void EndBoss::reset() {
    lives = 20;
    isAlive = true;
}
void EndBoss::render(Texture2D &tileAtlas) {
    DrawTexturePro(tileAtlas,{ srcRect.x + animationIndex * 32, srcRect.y + direction * 32, 32, 32 },{ pos.x, pos.y, 32, 32 },{ 0, 0 },0,DARKPURPLE);
}

int EndBoss::getDamage() {
    return m_damage;
}