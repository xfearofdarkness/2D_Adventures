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

    ChestUI(Inventory &chestInv, Inventory &playerInv);

    void update(float deltaTime);
    void render();
    bool shouldClose;
private:

    void transferItem();


    Inventory &chestInventory;
    Inventory &playerInventory;


    int chestSelectedIndex;
    int playerSelectedIndex;

    // Grid layout settings
    int columns;
    int chestRows;
    int playerRows;

    bool chestActive;
};

#endif // CHESTUI_H