//
// Created by jamie on 04.03.2025.
//

#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include "raylib.h"
#include "Inventory.h"

class DroppedItem {
public:
    DroppedItem(Item item, Vector2 position)
        : item(std::move(item)), position(position) {}

    void render() const {
        DrawTexture(item.icon, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
    }

    Rectangle getBoundingBox() const {
        return { position.x, position.y, 32, 32 };
    }

    Item item;
    Vector2 position;
};

#endif // DROPPEDITEM_H