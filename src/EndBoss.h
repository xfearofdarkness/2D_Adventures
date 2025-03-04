//
// Created by jamie on 04.03.2025.
//

#ifndef ENDBOSS_H
#define ENDBOSS_H

#include "Enemy.h"

class EndBoss : public Enemy {
public:
    EndBoss(Vector2 pos, Level &level);
    virtual void update(const Vector2 &playerPosition, float deltaTime) override;
    virtual void render(Texture2D &tileAtlas);
};

#endif // ENDBOSS_H
