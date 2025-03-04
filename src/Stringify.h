//
// Created by jamie on 02.03.2025.
//

#ifndef STRINGIFY_H
#define STRINGIFY_H
#include "Inventory.h"


constexpr std::string_view toString(ItemType type) {
    switch (type) {
        case ItemType::WOOD: return "Wood";
        case ItemType::STONE: return "Stone";
        case ItemType::CRAFTING_BENCH: return "Crafting Bench";
        case ItemType::CHEST: return "Chest";
        case ItemType::APPLE: return "Apple";
        case ItemType::SWORD: return "Sword";
        default: return "Unknown";
    }
}
#endif //STRINGIFY_H
