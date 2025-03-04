//
// Created by jamie on 03.01.2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include <vector>


#include "Level.h"
#include "raylib.h"
#include "Tile.h"

enum class EnemyState {
    WANDERING,
    CHASING
};
class Player;

class Enemy {
public:
    virtual ~Enemy() = default;

    Enemy(Vector2 position, Level &level);

    virtual void setState(EnemyState state);

    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0;
    float animationTimer = 0.0f;
    Vector2 pos;
    Rectangle srcRect = { 0, 32, 32, 32 };
    int lives = 4;


    [[nodiscard]] virtual Rectangle getBoundingBox() const {
        return { pos.x+4, pos.y+2, 24, 28 };  // Define the enemy's collision box
    }
    virtual void update(Player &player, float deltaTime);
    void moveTowardPlayer(const Vector2& playerPosition, float deltaTime);

    [[nodiscard]] bool checkCollisionWithPlayer(const Rectangle &playerBoundingBox) const;

    virtual bool checkCollision(Vector2 testPos);
    virtual void takeDamage(int damage);
    virtual void moveWithCollision(Vector2 moveVec);
    virtual void attack(Player &player, int damage);

    bool isAlive = true;
protected:
    Level *m_level;


    float m_speed = 75.0f;



    float stepCount = 0.0f;

    EnemyState m_state = EnemyState::WANDERING;

    float m_stateTimer = 0.0f;

private:
};






#endif //ENEMY_H
