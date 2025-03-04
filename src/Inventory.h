//
// Created by Jamie Huta on 26.02.25.
//

#ifndef INVENTORY_H
#define INVENTORY_H
#include <functional>
#include <raylib.h>
#include <string_view>
#include <utility>
#include <vector>

enum class ItemType {
    NONE,
    WOOD,
    STONE,
    CHEST,
    CRAFTING_BENCH,
    SWORD,
    APPLE
};

struct Item {
    ItemType type;
    mutable int quantity{};
    Texture2D icon{};



    std:: function<void()> onUse;
    Item() = default;
    // Constructor to initialize normal items
    Item(const ItemType type, const int quantity, const Texture2D &icon)
        : type(type), quantity(quantity), icon(icon), onUse(nullptr) {}
    // Constructor for special items that have a use effect
    Item(const ItemType type, const Texture2D &icon, std::function<void()> useFunction)
        : type(type), quantity(1), icon(icon), onUse(std::move(useFunction)) {}

    // Call this when the item is used
    void use() const {
        if (onUse) onUse();
    }
};

class Inventory {
public:
    Inventory() = default;
    explicit Inventory(std::vector<Item> items) : m_items(std::move(items)) {};

    void addItem(const Item& item);
    void removeItem(ItemType type, int amount);

    bool hasItem(ItemType type, int requiredQuantity) const;

    void printInventory() const;

    void render(Item *selected);

    [[nodiscard]] std::vector<Item> &getItems() { return m_items; }

    void reset();

public:

private:
    std::vector<Item> m_items;
};



#endif //INVENTORY_H
