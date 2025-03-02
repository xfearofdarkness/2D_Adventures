#ifndef CRAFTING_H
#define CRAFTING_H

#include <algorithm>
#include <vector>
#include "Inventory.h"
#include "raylib.h"
#include "Stringify.h"  // Contains toString(ItemType) for stringification

struct CraftingRecipe {
    ItemType outputType;
    int outputQuantity;
    std::vector<std::pair<ItemType, int>> ingredients;

    CraftingRecipe(const ItemType output, const int quantity, std::vector<std::pair<ItemType, int>> ing)
        : outputType(output), outputQuantity(quantity), ingredients(std::move(ing)) {}
};

inline std::vector<CraftingRecipe> recipes = {
    {ItemType::CRAFTING_BENCH, 1, {{ItemType::WOOD, 4}}},                 // 4 Wood → 1 Crafting Bench
    {ItemType::CHEST, 1, {{ItemType::WOOD, 8}, {ItemType::STONE, 2}}},      // 8 Wood + 2 Stone → 1 Chest
    {ItemType::SWORD, 1, {{ItemType::STONE, 2}, {ItemType::WOOD, 4}}}        // 4 Wood + 2 Stone → 1 Sword
};

class Crafting {
public:
    // Checks if inventory has enough items for the recipe.
    static bool canCraftRecipe(const CraftingRecipe& recipe, const Inventory& inventory);

    // Processes crafting: removes ingredients and adds the crafted item.
    static bool craftRecipe(const CraftingRecipe& recipe, Inventory& inventory);

    // Convenience: given an output type, finds the recipe and attempts to craft it.
    static bool craftItemFromType(ItemType outputType, Inventory& inventory);

    // Renders a crafting UI panel (for crafting bench) with available recipes.
    static void renderCraftingUI(const Inventory& inventory, const Vector2& uiPosition);

    // Renders a chest UI panel (for a chest) showing its contents.
    static void renderChestUI(const Inventory& chestInventory, const Vector2& uiPosition);

private:
    // Helper function: returns a texture for a given item type.
    // (Note: In a production system, load and cache textures once.)
    static Texture2D getTextureForItem(ItemType type);
};

#endif // CRAFTING_H
