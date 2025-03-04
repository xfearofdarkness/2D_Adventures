#include "Player.h"
#include <cmath>
#include <iostream>
#include "Crafting.h"
#include "Assert.h"

Player::Player(Vector2 position, Level &level)
    : pos(position), m_level(&level), m_selectedItem(nullptr), attackBoxRec({0, 0, 0, 0}), craftingBench(), chest() {
    state = PlayerState::IDLE;
    startPos = position;
}

Player::~Player() {
    // Unload textures if you own them:
    UnloadTexture(craftingBenchTexture);
    UnloadTexture(chestTexture);
    UnloadTexture(swordTexture);
    UnloadTexture(stoneTexture);
    UnloadTexture(filledHeart);
    UnloadTexture(emptyHeart);
    UnloadTexture(emptyLightning);
    UnloadTexture(filledLightning);
    UnloadTexture(woodTexture);
}

void Player::renderHUD() {
    if (craftingUI) {
        Crafting::updateCraftingUI(m_inventory);

        // Render the crafting UI at a desired position:
        Vector2 craftingUIPanelPos = {100, 100};
        Crafting::renderCraftingUI(m_inventory, craftingUIPanelPos);
    }
    const int iconSize = 32;
    const int spacing = 4;          // Spacing between hearts

    for (int i = 0; i < m_maxHealth; i++) {
        float x = 0 + i * (iconSize + spacing);
        // Draw a filled heart if this index is less than currentHealth, else an empty heart.
        if (i < m_health) {
            DrawTexture(filledHeart, x, 0, WHITE);
        } else {
            DrawTexture(emptyHeart, x, 0, WHITE);
        }
    }

    for (int i = 0; i < static_cast<int>(m_maxStamina) ; i++) {
        float x = 0 + i * (iconSize + spacing);
        bool next = m_stamina > i || i == m_maxStamina;
        if (i < m_stamina && next) {
            DrawTexture(filledLightning, x, iconSize+spacing, WHITE);
        } else {
            DrawTexture(emptyLightning, x, iconSize+spacing, WHITE);
        }
    }

    if (showChestUI) {
        chestUI->render();
    }
}

void Player::handleSelection() {

    if (!getPaused()) {
        if (IsKeyPressed(KEY_ONE)) {
            selectItem(0);
        }
        if (IsKeyPressed(KEY_TWO)) {
            selectItem(1);
        }
        if (IsKeyPressed(KEY_THREE)) {
            selectItem(2);
        }
        if (IsKeyPressed(KEY_FOUR)) {
            selectItem(3);
        }
        if (IsKeyPressed(KEY_FIVE)) {
            selectItem(4);
        }
        if (IsKeyPressed(KEY_SIX)) {
            selectItem(5);
        }
    }


    if (IsKeyPressed(KEY_E) && m_selectedItem) {
        m_selectedItem->use();
    }

    if (IsKeyPressed(KEY_F)) {
        m_inventory.printInventory();
    }
}

void Player::update(float deltaTime, std::vector<Enemy>& enemies) {
    if (state == PlayerState::DEAD) {
        return;
    }

    if (processMovement) {
        move(deltaTime);
        attack(enemies, deltaTime);
    }

    handleSelection();

    if (m_attackAnimationTimer > 0.0f) {
        m_attackAnimationTimer -= deltaTime;
        // Optionally, keep state as ATTACKING while timer is positive.
        if (m_attackAnimationTimer <= 0.0f && state == PlayerState::ATTACKING) {
            state = PlayerState::IDLE;
        }
    }
    if (IsKeyDown(KEY_Q)) {
        if (craftingUI && !showChestUI) craftingUI = false;
    }
    // Regenerate stamina continuously.
    m_stamina += m_staminaRegenRate * deltaTime;
    if (m_stamina > m_maxStamina) {
        m_stamina = m_maxStamina;
    }
    if (chestUI->shouldClose) showChestUI = false;
    if (showChestUI) chestUI->update(deltaTime);
}
void Player::initInventory() {
    loadItemTextures();
    craftingBench = {ItemType::CRAFTING_BENCH, craftingBenchTexture, [this] { openCraftingBench(); }};
    chest = {ItemType::CHEST, chestTexture, [this] { openChest(); }};
    testItem = {ItemType::STONE, 3, stoneTexture};
    testItem2 = {ItemType::WOOD, 4, woodTexture};
    assert(craftingBench.icon.id);
    m_inventory.addItem(craftingBench);
    m_inventory.addItem(testItem);
    m_inventory.addItem(chest);
    m_inventory.addItem(testItem2);
    chestUI = std::make_unique<ChestUI>(m_chestInventory, m_inventory);
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
    if (craftingUI || showChestUI) {
        return;
    }
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
                x >= static_cast<int>(m_level->GetTileMap()[0].size()) ||
                y >= static_cast<int>(m_level->GetTileMap().size())) {
                return true; // Collision with map bounds
            }

            if (isSolid(m_level->GetTileMap()[y][x])) {
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
    // Use discrete input so each press counts only once.
    bool attackPressed = IsKeyPressed(KEY_C) || IsKeyPressed(KEY_RIGHT_CONTROL) || IsKeyPressed(KEY_LEFT_CONTROL);

    // If not pressed or if we don't have at least 1 stamina, do nothing.
    if (!attackPressed || m_stamina < 1.0f) {
        state = PlayerState::IDLE;
        return;
    }

    // Attack occurs: subtract a fixed cost of 1 stamina.
    m_stamina -= 1.0f;
    state = PlayerState::ATTACKING;

    // Define the attack box based on the player's direction.
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
            break;
    }
    int itemDamage = 0;

    if (m_selectedItem->type == ItemType::SWORD) {
        itemDamage = 3;
    }
    // Damage enemies that collide with the attack box.
    for (Enemy& e : enemies) {
        if (e.isAlive && CheckCollisionRecs(attackBox, e.getBoundingBox())) {
            e.takeDamage(1+itemDamage);
        }
    }

    // Determine which tile is being attacked using the center of the attack box.
    int tileX = static_cast<int>((attackBox.x + attackBox.width / 2) / 32);
    int tileY = static_cast<int>((attackBox.y + attackBox.height / 2) / 32);

    // Delegate world interaction (e.g. chopping trees) to your handler.
    handleWorldInteraction(m_level->getTileAt(tileX * 32, tileY * 32),
                           { static_cast<float>(tileX * 32), static_cast<float>(tileY * 32) });

    // Store the attack box (e.g. for rendering the attack sprite).
    attackBoxRec = attackBox;
}


void Player::handleWorldInteraction(TileType tile, Vector2 worldPos) {
    switch (tile) {
        case TileType::Tree:
            m_level->SetTileAt(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y), TileType::Grass);
            //m_stamina--;
            //std::cout << "bruh you outa breath: " << m_stamina << std::endl;
            break;
        case TileType::Stone:
            m_level->SetTileAt(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y), TileType::Dirt);
            //m_stamina--;
            //std::cout << "bruh you outa breath: " << m_stamina << std::endl;
            break;
        default:
            break;
    }
}

void Player::renderInventory() {
    m_inventory.render(m_selectedItem);
}

void Player::loadItemTextures() {
    craftingBenchTexture = LoadTexture("res/Workbench.png");
    chestTexture = LoadTexture("res/Chest.png");
    swordTexture = LoadTexture("res/Sword.png");
    stoneTexture = LoadTexture("res/Stone.png");
    filledHeart = LoadTexture("res/FilledHeart.png");
    emptyHeart = LoadTexture("res/EmptyHeart.png");
    emptyLightning = LoadTexture("res/EmptyLightning.png");
    filledLightning = LoadTexture("res/FilledLightning.png");
    woodTexture = LoadTexture("res/Wood.png");
}

void Player::openChest() {
    chestUI->shouldClose = false;
    showChestUI = true;
}

void Player::openCraftingBench() {
    craftingUI = true;
}

void Player::selectItem(int index) {
    if (index >= 0 && index < static_cast<int>(m_inventory.getItems().size())) {
        m_selectedItem = &m_inventory.getItems()[index];
        std::cout << "Selected Item: " << toString(m_selectedItem->type) << " at index: " << index << "\n";
    } else {
        m_selectedItem = nullptr;  // Deselect if the index is out of range
        std::cout << "No item selected.\n" << std::endl;
    }
}
void Player::renderAttack(Texture2D &tileAtlas) const {
    if (state == PlayerState::ATTACKING || m_attackAnimationTimer > 0.0f)
        DrawRectangleLines((int)attackBoxRec.x, (int)attackBoxRec.y, (int)attackBoxRec.width, (int)attackBoxRec.height, YELLOW);
    if (state == PlayerState::ATTACKING || m_attackAnimationTimer > 0.0f)
        DrawTexturePro(tileAtlas, { attackSrcRect.x + static_cast<float>(direction * 32) , attackSrcRect.y, 32, 32}, attackBoxRec, { 0,0 }, 0, WHITE);
}
void Player::reset() {
    pos = startPos;
    direction = 0;
    m_health = 8;
    m_isAlive = true;
    m_attackCooldownTimer = 0.0f;
    m_stamina = 8.0f;
    m_inventory.reset();
    m_selectedItem = nullptr;
    craftingUI = false;
    showChestUI = false;
    state = PlayerState::IDLE;
    chestUI = nullptr;
    processMovement = true;
    initInventory();
}



