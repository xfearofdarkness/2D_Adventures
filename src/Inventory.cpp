//
// Created by Jamie Huta on 26.02.25.
//

#include "Inventory.h"

#include <iostream>
#include <string_view>

#include "Stringify.h"

void Inventory::addItem(const Item &newItem) {
    for (auto &existingItem : m_items) {
        // Compare only item type (and maybe texture id if that's constant for that item)
        if (existingItem.type == newItem.type) {
            existingItem.quantity += newItem.quantity;
            return;
        }
    }
    m_items.push_back(newItem);
}

void Inventory::removeItem(ItemType type, int amount) {
    for (auto it = m_items.begin(); it != m_items.end(); /* no increment here */) {
        if (it->type == type) {
            it->quantity -= amount;
            if (it->quantity <= 0) {
                it = m_items.erase(it);
            } else {
                ++it;
            }
            return;
        } else {
            ++it;
        }
    }
}

bool Inventory::hasItem(ItemType type, int requiredQuantity) const {
    for (const auto& item : m_items) {
        if (item.type == type && item.quantity >= requiredQuantity) {
            return true;
        }
    }
    return false;
}

void Inventory::printInventory() const {
    std::cout << "Inventory:" << std::endl;
    for (const auto& item : m_items) {
        std::cout << "- " << toString(item.type)
                  << " x" << item.quantity << std::endl;
    }
}

void Inventory::render(Item* selected) {
    int segments = 6;               // Number of inventory slots
    float height = 64;              // Total hotbar height
    float slotSize = 56.0f;         // Slot size
    float slotPadding = 4.0f;       // Padding between slots
    float width = (slotSize + slotPadding) * segments - slotPadding; // Correct total width now!

    float startRecX = static_cast<float>(GetScreenWidth() / 2) - width / 2;
    float startRecY = static_cast<float>(GetScreenHeight() - height);

    // Background bar
    DrawRectangleRounded({startRecX, startRecY, width, height}, 0.5f, 4, BLACK);

    for (int i = 0; i < segments; ++i) {
        float slotX = startRecX + i * (slotSize + slotPadding);
        float slotY = startRecY + slotPadding;

        // Draw slot frame
        DrawRectangleRounded({slotX, slotY, slotSize, slotSize}, 0.25f, 6, GRAY);

        // Draw selection highlight
        if (i < static_cast<int>(m_items.size()) && &m_items[i] == selected) {
            DrawRectangleRounded({slotX, slotY, slotSize, slotSize}, 0.25f, 6, YELLOW);
        }

        // Draw the item icon if present
        if (i < static_cast<int>(m_items.size())) {
            // Centering the 32x32 icon within the 56x56 slot
            float iconX = slotX + (slotSize - 32) / 2;
            float iconY = slotY + (slotSize - 32) / 2;
            DrawTexture(m_items[i].icon, iconX, iconY, WHITE);

            // Optionally draw the quantity (if more than one)
            if (m_items[i].quantity > 1) {
                DrawText(TextFormat("%d", m_items[i].quantity), iconX + 2, iconY + 2, 20, WHITE);
            }
        }
    }
}

void Inventory::reset() {
    m_items.clear();
}

