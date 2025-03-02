//
// Created by Jamie Huta on 26.02.25.
//

#ifndef INVENTORY_H
#define INVENTORY_H
#include <functional>
#include <raylib.h>
#include <utility>
#include <vector>

enum class ItemType {
    NONE,
    WOOD,
    STONE,
    CHEST,
    CRAFTING_BENCH,
    SWORD
};

struct CraftingRecipe {
    ItemType outputType;
    int outputQuantity;
    std::vector<std::pair<ItemType, int>> ingredients;

    CraftingRecipe(const ItemType output, const int quantity, std::vector<std::pair<ItemType, int>> ing)
        : outputType(output), outputQuantity(quantity), ingredients(std::move(ing)) {}
};

inline std::vector<CraftingRecipe> recipes = {
    {ItemType::CRAFTING_BENCH, 1, {{ItemType::WOOD, 4}}},   // 4 Wood → 1 Crafting Bench
    {ItemType::CHEST, 1, {{ItemType::WOOD, 8}, {ItemType::STONE, 2}}} // 8 Wood + 2 Stone → 1 Chest
};

struct Item {
    ItemType type;
    int quantity;
    Texture2D icon;


    std::function<void()> onUse;

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

    [[nodiscard]] const std::vector<Item>& getItems() const { return m_items; }
public:

private:
    std::vector<Item> m_items;
};



#endif //INVENTORY_H
