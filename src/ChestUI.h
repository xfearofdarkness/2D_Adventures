//
// Created by jamie on 03.03.2025.
//

#ifndef CHESTUI_H
#define CHESTUI_H

#include "Inventory.h"
#include "raylib.h"
#include <vector>
#include <string>

enum class ActiveGrid {
    CHEST,
    PLAYER
};

class ChestUI {
public:
    // Construct the ChestUI with references to both inventories.
    ChestUI(Inventory &chestInv, Inventory &playerInv);

    // Call this every frame to handle input.
    void update(float deltaTime);
    // Call this every frame to render the UI.
    void render();

private:
    Inventory &chestInventory;
    Inventory &playerInventory;
    ActiveGrid activeGrid;

    // Selected index for each grid.
    int chestSelectedIndex;
    int playerSelectedIndex;

    // Grid dimensions.
    int chestColumns; // e.g., 6 columns
    int chestRows;    // e.g., 4 rows (24 slots total)
    int playerColumns; // e.g., 6 columns
    int playerRows;    // e.g., 1 row

    // Helper functions:
    void transferItem();       // Moves the selected item from one inventory to the other.
    void updateSelection();    // Handles arrow key input to update the selection.
};

#endif // CHESTUI_H