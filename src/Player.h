//
// Created by jamie on 03.01.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>


#include "ChestUI.h"
#include "Crafting.h"
#include "DroppedItem.h"
#include "raylib.h"
#include "Tile.h"
#include "Enemy.h"
#include "Inventory.h"
#include "Level.h"
#include "Stringify.h"
#include "raymath.h"
#include "Slime.h"
class Enemy;

enum class PlayerState {
    IDLE,
    ATTACKING,
    DEAD,
};

class Player {
    public:
    Player(Vector2 pos, Level &level, std::vector<DroppedItem> *droppedItems);
    ~Player();
    void renderHUD();

    void handleSelection();
    [[nodiscard]] bool getPaused() const {
        return showChestUI || craftingUI;
    }

    Vector2 pos;
    Vector2 startPos;
    int direction = 0; // 0 = down, 1 = up, 2 = left, 3 = right
    int animationIndex = 0; // Animation frame index
    float animationTimer = 0.0f; // Tracks time for animation updates
    float stepCount = 0.0f; // Tracks distance traveled for animation purposes

    Rectangle srcRect = { 0, 32, 32, 32 };
    Rectangle attackSrcRect = { 0, 160, 32, 32 };

    [[nodiscard]] Rectangle getBoundingBox() const {
        return { pos.x+4, pos.y+2, 24, 28 }; // Define the player's collision box
    }

    void update(float deltaTime, std::vector<Enemy> &enemies, std::vector<Slime> &slimes);

    void initInventory();

    void takeDamage(int damage);

    void heal(int amount);

    void useSelectedItem();

    void attack(std::vector<Enemy>& enemies, std::vector<Slime>& slimes, float delta_time);

    void handleWorldInteraction(TileType tile, Vector2 worldPos);

    void renderInventory();
    void loadItemTextures();
    void reset();

    Rectangle attackBoxRec;
    PlayerState state;
    bool processMovement = true;


    void renderAttack(Texture2D &tileAtlas);

private:
    void move(float deltaTime);

    void moveWithCollision(Vector2 moveVec);

    bool checkCollision(Vector2 testPos);

    void openChest();

    void openCraftingBench();

    void selectItem(int index);

private:
    float m_attackAnimationTimer = 0.0f;           // Time remaining for attack animation to display
    const float m_attackAnimationDuration = 0.4f;
    // Textures for items (private)
    Texture2D craftingBenchTexture;
    Texture2D chestTexture;
    Texture2D swordTexture;
    Texture2D stoneTexture;
    Texture2D filledHeart;
    Texture2D emptyHeart;
    Texture2D emptyLightning;
    Texture2D filledLightning;
    Texture2D woodTexture;
    Texture2D appleTexture;
    Crafting craftingEntity;
    // Inventory items
    Item craftingBench;
    Item chest;
    //other members
    std::unique_ptr<ChestUI> chestUI;
    Level *m_level;
    std::vector<DroppedItem> *m_droppedItems;
    int m_health = 8;
    const int m_maxHealth = 8;
    bool m_isAlive = true;
    float m_stamina = 8.0f;               // Current stamina (max 8)
    const float m_maxStamina = 8.0f;        // Maximum stamina
    const float m_staminaCost = 1.0f;       // Each attack costs 1 stamina
    const float m_staminaRegenRate = 1.0f;  // Regenerate 1 stamina per second
    float m_attackCooldownTimer = 0.0f;     // Timer to enforce attack cooldown
    const float m_attackCooldown = 0.7f;
    Inventory m_inventory;
    Item *m_selectedItem;
    bool craftingUI = false;
    bool showChestUI = false;
    Inventory m_chestInventory;
    Item testItem;
    Item testItem2;
};





#endif // PLAYER_H

