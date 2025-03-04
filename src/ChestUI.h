//
// Created by jamie on 03.03.2025.
//

#ifndef CHESTUI_H
#define CHESTUI_H

#include "Inventory.h"
#include "raylib.h"
#include <memory>

class ChestUI {
public:
    // Construct the ChestUI with references to the chest and player inventories.
    ChestUI(Inventory &chestInv, Inventory &playerInv);

    // Update and render the unified grid.
    void update(float deltaTime);
    void render();
    bool shouldClose;
private:
    // Helper: Transfer the selected item from one inventory to the other.
    void transferItem();

    // References to the inventories.
    Inventory &chestInventory;
    Inventory &playerInventory;

    // Use separate selection indices.
    int chestSelectedIndex; // for the chest inventory grid
    int playerSelectedIndex; // for the player's inventory grid

    // Grid layout settings.
    int columns;       // e.g., 6 columns for both grids
    int chestRows;     // e.g., 4 rows for chest
    int playerRows;    // e.g., 1 row for player's inventory

    // Flag to determine which grid is active.
    // true means chest grid is active; false means player's inventory is active.
    bool chestActive;
};

#endif // CHESTUI_H