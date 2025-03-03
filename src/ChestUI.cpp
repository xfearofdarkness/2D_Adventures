//
// Created by jamie on 03.03.2025.
//

#include "ChestUI.h"

#include "raylib.h"
#include <iostream>

#include "Stringify.h"

ChestUI::ChestUI(Inventory &chestInv, Inventory &playerInv)
    : chestInventory(chestInv), playerInventory(playerInv), activeGrid(ActiveGrid::CHEST),
      chestSelectedIndex(0), playerSelectedIndex(0),
      chestColumns(6), chestRows(4),
      playerColumns(6), playerRows(1)
{
    // You can initialize grid dimensions or other variables here.
}

void ChestUI::update(float deltaTime) {
    // Toggle active grid using Tab.
    if (IsKeyPressed(KEY_TAB)) {
        activeGrid = (activeGrid == ActiveGrid::CHEST) ? ActiveGrid::PLAYER : ActiveGrid::CHEST;
    }

    // Update selection based on active grid:
    if (activeGrid == ActiveGrid::CHEST) {
        // For the chest grid: update index with arrow keys.
        int numSlots = chestColumns * chestRows;
        // Ensure we don't go beyond the number of items available.
        if (!chestInventory.getItems().empty()) {
            if (IsKeyPressed(KEY_RIGHT)) {
                chestSelectedIndex = (chestSelectedIndex + 1) % numSlots;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                chestSelectedIndex = (chestSelectedIndex - 1 + numSlots) % numSlots;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                chestSelectedIndex = (chestSelectedIndex + chestColumns) % numSlots;
            }
            if (IsKeyPressed(KEY_UP)) {
                chestSelectedIndex = (chestSelectedIndex - chestColumns + numSlots) % numSlots;
            }
        }
    } else { // Active grid is PLAYER.
        int numSlots = playerColumns * playerRows;
        if (!playerInventory.getItems().empty()) {
            if (IsKeyPressed(KEY_RIGHT)) {
                playerSelectedIndex = (playerSelectedIndex + 1) % numSlots;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                playerSelectedIndex = (playerSelectedIndex - 1 + numSlots) % numSlots;
            }
            // For the player's inventory, we assume one row so UP/DOWN might not change selection.
        }
    }

    // If Enter is pressed, transfer the currently selected item.
    if (IsKeyPressed(KEY_ENTER)) {
        transferItem();
    }
}

void ChestUI::transferItem() {
    // If active grid is CHEST, move item from chest to player's inventory.
    if (activeGrid == ActiveGrid::CHEST) {
        auto &chestItems = chestInventory.getItems();
        if (!chestItems.empty() && chestSelectedIndex < static_cast<int>(chestItems.size())) {
            // Get the selected item.
            Item itemToTransfer = chestItems[chestSelectedIndex];

            // Check if the item is a chest; if so, skip transfer.
            if (itemToTransfer.type == ItemType::CHEST) {
                std::cout << "Cannot transfer a chest item." << std::endl;
                return;
            }

            // Remove the entire stack from the chest.
            chestInventory.removeItem(itemToTransfer.type, itemToTransfer.quantity);
            // Add it to the player's inventory.
            playerInventory.addItem(itemToTransfer);
            std::cout << "Transferred " << itemToTransfer.quantity << " of "
                      << toString(itemToTransfer.type) << " from chest to inventory.\n";
            // Update the selection index.
            if (chestSelectedIndex >= static_cast<int>(chestInventory.getItems().size())) {
                chestSelectedIndex = std::max(0, static_cast<int>(chestInventory.getItems().size()) - 1);
            }
        }
    }
    // If active grid is PLAYER, move item from player's inventory to chest.
    else {
        auto &playerItems = playerInventory.getItems();
        if (!playerItems.empty() && playerSelectedIndex < static_cast<int>(playerItems.size())) {
            Item itemToTransfer = playerItems[playerSelectedIndex];

            // Check if the item is a chest; if so, skip transfer.
            if (itemToTransfer.type == ItemType::CHEST) {
                std::cout << "Cannot transfer a chest item." << std::endl;
                return;
            }

            playerInventory.removeItem(itemToTransfer.type, itemToTransfer.quantity);
            chestInventory.addItem(itemToTransfer);
            std::cout << "Transferred " << itemToTransfer.quantity << " of "
                      << toString(itemToTransfer.type) << " from inventory to chest.\n";
            if (playerSelectedIndex >= static_cast<int>(playerInventory.getItems().size())) {
                playerSelectedIndex = std::max(0, static_cast<int>(playerInventory.getItems().size()) - 1);
            }
        }
    }
}

void ChestUI::render() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Define panel sizes.
    int chestPanelWidth = 440;
    int chestPanelHeight = 300;
    int playerPanelWidth = 440;
    int playerPanelHeight = 80;

    // Calculate panel positions (centered).
    Rectangle chestPanel = {
        static_cast<float>(screenWidth / 2 - chestPanelWidth / 2),
        static_cast<float>(screenHeight / 2 - chestPanelHeight / 2 - 50),
        static_cast<float>(chestPanelWidth),
        static_cast<float>(chestPanelHeight)
    };

    Rectangle playerPanel = {
        static_cast<float>(screenWidth / 2 - playerPanelWidth / 2),
        static_cast<float>(screenHeight / 2 + chestPanelHeight / 2),
        static_cast<float>(playerPanelWidth),
        static_cast<float>(playerPanelHeight)
    };

    // Draw the panels.
    DrawRectangleRec(chestPanel, Fade(DARKGRAY, 0.8f));
    DrawRectangleRec(playerPanel, Fade(DARKGRAY, 0.8f));

    int slotSize = 64;
    int padding = 8;

    // Render chest inventory grid.
    for (int i = 0; i < chestColumns * chestRows; i++) {
        int col = i % chestColumns;
        int row = i / chestColumns;
        float slotX = chestPanel.x + padding + col * (slotSize + padding);
        float slotY = chestPanel.y + padding + row * (slotSize + padding);
        Rectangle slotRect = { slotX, slotY, static_cast<float>(slotSize), static_cast<float>(slotSize) };
        DrawRectangleRec(slotRect, LIGHTGRAY);
        if (activeGrid == ActiveGrid::CHEST && i == chestSelectedIndex) {
            DrawRectangleLinesEx(slotRect, 3, YELLOW);
        }
        // Draw item if available.
        const auto &chestItems = chestInventory.getItems();
        if (i < static_cast<int>(chestItems.size())) {
            const Item &item = chestItems[i];
            float iconX = slotX + (slotSize - 32) / 2.0f;
            float iconY = slotY + (slotSize - 32) / 2.0f;
            DrawTexture(item.icon, iconX, iconY, WHITE);
            if (item.quantity > 1) {
                DrawText(TextFormat("%d", item.quantity), slotX + 5, slotY + 5, 20, WHITE);
            }
        }
    }

    // Render player's inventory grid.
    for (int i = 0; i < playerColumns * playerRows; i++) {
        int col = i % playerColumns;
        int row = i / playerColumns;
        float slotX = playerPanel.x + padding + col * (slotSize + padding);
        float slotY = playerPanel.y + padding + row * (slotSize + padding);
        Rectangle slotRect = { slotX, slotY, static_cast<float>(slotSize), static_cast<float>(slotSize) };
        DrawRectangleRec(slotRect, LIGHTGRAY);
        if (activeGrid == ActiveGrid::PLAYER && i == playerSelectedIndex) {
            DrawRectangleLinesEx(slotRect, 3, YELLOW);
        }
        const auto &playerItems = playerInventory.getItems();
        if (i < static_cast<int>(playerItems.size())) {
            const Item &item = playerItems[i];
            float iconX = slotX + (slotSize - 32) / 2.0f;
            float iconY = slotY + (slotSize - 32) / 2.0f;
            DrawTexture(item.icon, iconX, iconY, WHITE);
            if (item.quantity > 1) {
                DrawText(TextFormat("%d", item.quantity), slotX + 5, slotY + 5, 20, WHITE);
            }
        }
    }
}