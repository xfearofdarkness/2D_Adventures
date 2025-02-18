#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(Vector2 pos, const std::vector<std::vector<TileType>>& tilemap)
    : pos(pos), m_tilemap(tilemap) {}

Player::~Player() {}

void Player::update(float deltaTime, std::vector<Enemy>& enemies) {
    move(deltaTime);
    attack(enemies);
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        isAlive = false;
    }
}

void Player::move(float deltaTime) {
    bool up = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool down = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    bool left = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

    float speed = 100.0f;
    bool moved = false;

    // Cancel out opposing directions
    if (up && down) up = down = false;
    if (left && right) left = right = false;

    Vector2 moveVec = {0.0f, 0.0f};

    // Calculate movement vector
    if (up) {
        moveVec.y -= speed * deltaTime;
        this->direction = 1;
        moved = true;
    }
    if (down) {
        moveVec.y += speed * deltaTime;
        this->direction = 0;
        moved = true;
    }
    if (left) {
        moveVec.x -= speed * deltaTime;
        this->direction = 2;
        moved = true;
    }
    if (right) {
        moveVec.x += speed * deltaTime;
        this->direction = 3;
        moved = true;
    }

    // Try to move with collision resolution
    moveWithCollision(moveVec);

    // Animation update
    if (moved) {
        this->stepCount += speed * deltaTime;
        if (this->stepCount > 16.0f) {
            this->stepCount = 0.0f;
            this->animationIndex = (this->animationIndex + 1) % 2;
        }
    }
}

void Player::moveWithCollision(Vector2 moveVec) {
    Vector2 targetPos = {
        this->pos.x + moveVec.x,
        this->pos.y + moveVec.y
    };

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

bool Player::checkCollision(Vector2 testPos) {
    //adjust player bounds for sprite offset
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
                x >= static_cast<int>(m_tilemap[0].size()) ||
                y >= static_cast<int>(m_tilemap.size())) {
                return true; // Collision with map bounds
            }

            if (isSolid(m_tilemap[y][x])) {
                Rectangle tileRect = {
                    x * 32.0f,
                    y * 32.0f,
                    32.0f,
                    32.0f
                };

                if (CheckCollisionRecs(bounds, tileRect)) {
                    return true; // Collision detected
                }
            }
        }
    }

    return false; // No collision
}

void Player::attack(std::vector<Enemy>& enemies) {

    bool attacking = IsKeyDown(KEY_C) || IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);

    if (!attacking) return;

    Rectangle attackBox;

    switch (direction) {
        case 0: // Down
            attackBox = { pos.x + 4, pos.y + 32, 24, 16 };
        break;
        case 1: // Up
            attackBox = { pos.x + 4, pos.y - 16, 24, 16 };
        break;
        case 2: // Left
            attackBox = { pos.x - 16, pos.y + 2, 16, 28 };
        break;
        case 3: // Right
            attackBox = { pos.x + 32, pos.y + 2, 16, 28 };
        break;
        default:
            attackBox = {};
    }
    for (Enemy& e : enemies) {
        if (e.isAlive && CheckCollisionRecs(attackBox, e.getBoundingBox())) {
            e.takeDamage(1);
        }
    }
    attackBoxRec = attackBox;
}

