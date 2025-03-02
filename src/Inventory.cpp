//
// Created by Jamie Huta on 26.02.25.
//

#include "Inventory.h"

void Inventory::addItem(const Item &newItem) {
    for (auto &item : m_items) {
        if (item.type == newItem.type) {
            item.quantity += newItem.quantity;
            return;
        }
    }
    m_items.push_back(newItem);
}

void Inventory::removeItem(const ItemType type, const int amount) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if (it->type == type) {
            it->quantity -= amount;
            if (amount <= 0) {
                m_items.erase(it);
            }
            return;
        }
    }
}