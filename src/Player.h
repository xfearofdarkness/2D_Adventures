//
// Created by jamie on 03.01.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>


#include "Crafting.h"
#include "raylib.h"
#include "Tile.h"
#include "Enemy.h"
#include "Inventory.h"
#include "Level.h"
#include "Stringify.h"

class Enemy;

enum class PlayerState {
    IDLE,
    ATTACKING,
    DEAD,
};

class Player {
    public:
    Player(Vector2 pos, Level &level);
    ~Player();

    void renderHUD();

    void handleSelection();

    Vector2 pos;
    Vector2 startPos = {360, 320};
    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0; // Animation frame index
    float animationTimer = 0.0f; // Tracks time for animation updates
    float stepCount = 0.0f; // Tracks distance traveled for animation purposes

    Rectangle srcRect = { 0, 32, 32, 32 };
    Rectangle attackSrcRect = { 0, 160, 32, 32 };

    [[nodiscard]] Rectangle getBoundingBox() const {
        return { pos.x+4, pos.y+2, 24, 28 }; // Define the player's collision box
    }

    void update(float deltaTime, std::vector<Enemy>& enemies);

    void initInventory();

    void takeDamage(int damage);
    void attack(std::vector<Enemy>& enemies, float delta_time);
    void renderInventory();
    void loadItemTextures();
    void reset();

    Rectangle attackBoxRec;
    PlayerState state;
private:
    void move(float deltaTime);

    void moveWithCollision(Vector2 moveVec);

    bool checkCollision(Vector2 testPos);

    void openChest();
    void openCraftingBench();

    void selectItem(int index);


private:
    // Textures for items (private)
    Texture2D craftingBenchTexture;
    Texture2D chestTexture;
    Texture2D swordTexture;
    Texture2D stoneTexture;
    Texture2D filledHeart;
    Texture2D emptyHeart;
    Crafting craftingEntity;
    // Inventory items
    Item craftingBench;
    Item chest;
    //other members

    Level &m_level;
    int m_health = 8;
    int m_maxHealth = 8;
    bool m_isAlive = true;
    float m_stamina_timer = 8.0f;
    Inventory m_inventory;
    Item *m_selectedItem;
    bool craftingUI = false;
    bool chestUI = false;


};





#endif // PLAYER_H

