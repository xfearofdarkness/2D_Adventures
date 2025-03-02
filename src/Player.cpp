#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(Vector2 pos, Level &level)
    : pos(pos), m_level(level), m_selectedItem(nullptr), attackBoxRec({0, 0, 0, 0}) {
    state = PlayerState::IDLE;
}

Player::~Player() {}

void Player::update(float deltaTime, std::vector<Enemy>& enemies) {
    move(deltaTime);
    attack(enemies, deltaTime);
}

void Player::takeDamage(int damage) {
    m_health -= damage;
    if (m_health <= 0) {
        m_health = 0;
        m_isAlive = false;
        state = PlayerState::DEAD;
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
                x >= static_cast<int>(m_level.GetTileMap()[0].size()) ||
                y >= static_cast<int>(m_level.GetTileMap().size())) {
                return true; // Collision with map bounds
            }

            if (isSolid(m_level.GetTileMap()[y][x])) {
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

void Player::attack(std::vector<Enemy>& enemies, float delta_time) {
    
    bool attacking = IsKeyDown(KEY_C) || IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
    if (attacking) m_stamina_timer -= 1.0f;
    if (m_stamina_timer < 8.0f && !attacking) {
        m_stamina_timer += 1.0f;
    }
    if (!attacking) {
        state = PlayerState::IDLE;
        return;
    }
    
    state = PlayerState::ATTACKING;
    Rectangle attackBox;

    switch (direction) {
        case 0: // Down
            attackBox = { pos.x, pos.y + 16, 32, 32 };
        break;
        case 1: // Up
            attackBox = { pos.x, pos.y - 16, 32, 32 };
        break;
        case 2: // Left
            attackBox = { pos.x - 16, pos.y + 2, 32, 32 };
        break;
        case 3: // Right
            attackBox = { pos.x + 16, pos.y + 2, 32, 32 };
        break;
        default:
            attackBox = {};
    }
    //A little inefficient, oops
    for (Enemy& e : enemies) {
        if (e.isAlive && CheckCollisionRecs(attackBox, e.getBoundingBox())) {
            e.takeDamage(1);
        }
    }
    int tileX = static_cast<int>((attackBox.x + attackBox.width / 2) / 32);
    int tileY = static_cast<int>((attackBox.y + attackBox.height / 2) / 32);

    // Instead of assuming the entire 32x32 tile is the hit area,
    // assume only the lower half (the trunk of a tree) is vulnerable.
    Rectangle treeCollisionRect = { tileX * 32.0f, tileY * 32.0f + 16.0f, 32.0f, 16.0f };

    // Check if the tile is a tree and if the attack box overlaps the tree's "hit" region.
    if (m_level.getTileAt(tileX * 32, tileY * 32) == TileType::Tree &&
        CheckCollisionRecs(attackBox, treeCollisionRect)) {
        m_level.SetTileAt(tileX * 32, tileY * 32, TileType::Grass);
        std::cout << "Tree chopped! Changed to Grass at: " << tileX << ", " << tileY << std::endl;
    }

    attackBoxRec = attackBox;
}



