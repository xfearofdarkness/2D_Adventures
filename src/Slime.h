//
// Created by jamie on 03.03.2025.
//

#ifndef SLIME_H
#define SLIME_H

#include "Enemy.h"

class Slime : public Enemy {
public:
    Slime(Vector2 pos, Level &level);
    virtual void update(const Vector2 &playerPosition, float deltaTime) override;
    virtual void render(Texture2D &tileAtlas);

    void setState(EnemyState state);

private:
    float m_animationTimer;             // Timer for cycling frames
    float m_animationSpeed = 0.2f;  // Seconds per frame (adjust as desired)
};

#endif // SLIME_H
