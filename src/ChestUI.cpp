//
// Created by jamie on 03.03.2025.
//

#include "ChestUI.h"

#include "raylib.h"
#include <iostream>

#include "Stringify.h"

#include "ChestUI.h"
#include "raylib.h"
#include <iostream>
#include <algorithm>

#include "ChestUI.h"
#include "raylib.h"
#include <iostream>
#include <algorithm>

ChestUI::ChestUI(Inventory &chestInv, Inventory &playerInv)
    : chestInventory(chestInv), playerInventory(playerInv),
      columns(6), chestRows(4), playerRows(1), chestActive(true)
{
    chestSelectedIndex = 0;
    playerSelectedIndex = 0;
    // If chest is empty at startup, start with player's inventory active.
    if (chestInventory.getItems().empty()) {
        chestActive = false;
    }
    shouldClose = false;
}

void ChestUI::update(float deltaTime) {

    if (IsKeyDown(KEY_Q)) {
        shouldClose = true;
    }
    // Navigation using arrow keys:
    int chestSlots = chestRows * columns;
    int playerSlots = playerRows * columns;

    if (chestActive) {
        // Navigate within the chest grid.
        if (IsKeyPressed(KEY_RIGHT)) {
            chestSelectedIndex = (chestSelectedIndex + 1) % chestSlots;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            chestSelectedIndex = (chestSelectedIndex - 1 + chestSlots) % chestSlots;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            // Move from chest grid to player's grid if on bottom row.
            if (chestSelectedIndex >= chestSlots - columns) {
                chestActive = false;
            } else {
                chestSelectedIndex = std::min(chestSlots - 1, chestSelectedIndex + columns);
            }
        }
        if (IsKeyPressed(KEY_UP)) {
            chestSelectedIndex = std::max(0, chestSelectedIndex - columns);
        }
    } else { // Player inventory active.
        if (IsKeyPressed(KEY_RIGHT)) {
            playerSelectedIndex = (playerSelectedIndex + 1) % playerSlots;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            playerSelectedIndex = (playerSelectedIndex - 1 + playerSlots) % playerSlots;
        }
        if (IsKeyPressed(KEY_UP)) {
            // Move from player grid to chest grid.
            playerSelectedIndex = 0;  // Optionally, set to a default value.
            chestActive = true;
        }
        // Down key: if needed, can wrap around within player's grid.
    }

    // Transfer item on Enter.
    if (IsKeyPressed(KEY_ENTER)) {
        transferItem();
    }
}

void ChestUI::transferItem() {
    // Transfer from chest to player's inventory if the chest grid is active.
    if (chestActive) {
        auto &chestItems = chestInventory.getItems();
        if (chestItems.empty() || chestSelectedIndex >= static_cast<int>(chestItems.size()))
            return;

        // Create a copy of the selected item.
        Item itemToTransfer = chestItems[chestSelectedIndex];
        int transferQuantity = itemToTransfer.quantity;

        // Prevent transferring a chest item or a zero-quantity item.
        if (itemToTransfer.type == ItemType::CHEST || transferQuantity <= 0) {
            std::cout << "Cannot transfer this item." << std::endl;
            return;
        }

        // Remove the item from the chest inventory.
        chestInventory.removeItem(itemToTransfer.type, transferQuantity);

        // Restore the original quantity in our copy.
        itemToTransfer.quantity = transferQuantity;
        // Add the copied item to the player's inventory.
        playerInventory.addItem(itemToTransfer);
        std::cout << "Transferred " << transferQuantity << " of "
                  << toString(itemToTransfer.type) << " from chest to inventory." << std::endl;

        // Adjust the chest selection index so it doesn't point to a shifted item.
        if (chestInventory.getItems().empty()) {
            chestSelectedIndex = 0;
        } else if (chestSelectedIndex >= static_cast<int>(chestInventory.getItems().size())) {
            chestSelectedIndex = static_cast<int>(chestInventory.getItems().size()) - 1;
        }
    }
    // Transfer from player's inventory to chest if the player's grid is active.
    else {
        auto &playerItems = playerInventory.getItems();
        if (playerItems.empty() || playerSelectedIndex >= static_cast<int>(playerItems.size()))
            return;

        Item itemToTransfer = playerItems[playerSelectedIndex];
        int transferQuantity = itemToTransfer.quantity;

        if (itemToTransfer.type == ItemType::CHEST || transferQuantity <= 0) {
            std::cout << "Cannot transfer this item." << std::endl;
            return;
        }

        // Remove the item from the player's inventory.
        playerInventory.removeItem(itemToTransfer.type, transferQuantity);
        itemToTransfer.quantity = transferQuantity;
        // Add the item to the chest inventory.
        chestInventory.addItem(itemToTransfer);
        std::cout << "Transferred " << transferQuantity << " of "
                  << toString(itemToTransfer.type) << " from inventory to chest." << std::endl;

        if (playerInventory.getItems().empty()) {
            playerSelectedIndex = 0;
        } else if (playerSelectedIndex >= static_cast<int>(playerInventory.getItems().size())) {
            playerSelectedIndex = static_cast<int>(playerInventory.getItems().size()) - 1;
        }
    }
}

void ChestUI::render() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Define panel sizes.
    Rectangle chestPanel = { static_cast<float>(screenWidth / 2 - 300),
                             static_cast<float>(screenHeight / 2 - 200),
                             440, 300 };
    Rectangle playerPanel = { static_cast<float>(screenWidth / 2 - 300),
                              static_cast<float>(screenHeight / 2 + 150),
                              440, 100 };

    // Draw panel backgrounds.
    DrawRectangleRec(chestPanel, Fade(DARKGRAY, 0.8f));
    DrawRectangleRec(playerPanel, Fade(DARKGRAY, 0.8f));

    int slotSize = 64;
    int padding = 8;

    // Render chest grid.
    int chestSlots = chestRows * columns;
    for (int i = 0; i < chestSlots; i++) {
        int col = i % columns;
        int row = i / columns;
        float slotX = chestPanel.x + padding + col * (slotSize + padding);
        float slotY = chestPanel.y + padding + row * (slotSize + padding);
        Rectangle slotRect = { slotX, slotY, static_cast<float>(slotSize), static_cast<float>(slotSize) };
        DrawRectangleRec(slotRect, LIGHTGRAY);
        if (chestActive && i == chestSelectedIndex) {
            DrawRectangleLinesEx(slotRect, 3, YELLOW);
        }
        const auto &chestItems = chestInventory.getItems();
        if (i < static_cast<int>(chestItems.size())) {
            const Item &item = chestItems[i];
            float iconX = slotX + (slotSize - 32) / 2.0f;
            float iconY = slotY + (slotSize - 32) / 2.0f;
            DrawTexture(item.icon, iconX, iconY, WHITE);
            if (item.quantity > 1) {
                DrawText(TextFormat("%d", item.quantity), slotX + slotSize - 20, slotY + slotSize - 22, 18, WHITE);
            }
        }
    }

    // Render player's inventory grid.
    int playerSlots = playerRows * columns;
    for (int i = 0; i < playerSlots; i++) {
        int col = i % columns;
        int row = i / columns;
        float slotX = playerPanel.x + padding + col * (slotSize + padding);
        float slotY = playerPanel.y + padding + row * (slotSize + padding);
        Rectangle slotRect = { slotX, slotY, static_cast<float>(slotSize), static_cast<float>(slotSize) };
        DrawRectangleRec(slotRect, LIGHTGRAY);
        if (!chestActive && i == playerSelectedIndex) {
            DrawRectangleLinesEx(slotRect, 3, YELLOW);
        }
        const auto &playerItems = playerInventory.getItems();
        if (i < static_cast<int>(playerItems.size())) {
            const Item &item = playerItems[i];
            float iconX = slotX + (slotSize - 32) / 2.0f;
            float iconY = slotY + (slotSize - 32) / 2.0f;
            DrawTexture(item.icon, iconX, iconY, WHITE);
            if (item.quantity > 1) {
                DrawText(TextFormat("%d", item.quantity), slotX + slotSize - 20, slotY + slotSize - 22, 18, WHITE);
            }
        }
    }
}