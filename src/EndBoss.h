//
// Created by jamie on 04.03.2025.
//

#ifndef ENDBOSS_H
#define ENDBOSS_H

#include "Enemy.h"

class EndBoss : public Enemy {
public:
    EndBoss(Vector2 pos, Level &level);
    virtual void update(Player &player, float deltaTime) override;

    void reset();

    virtual void render(Texture2D &tileAtlas);

    int getDamage();

private:
    int m_damage;
};

#endif // ENDBOSS_H
