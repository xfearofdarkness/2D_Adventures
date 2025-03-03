//
// Created by jamie on 02.03.2025.
//

#include "Crafting.h"
#include <string>
#include <sstream>

// Checks if inventory has enough items for the recipe.
bool Crafting::canCraftRecipe(const CraftingRecipe& recipe, const Inventory& inventory) {
    for (const auto& ingredient : recipe.ingredients) {
        if (!inventory.hasItem(ingredient.first, ingredient.second)) {
            return false;
        }
    }
    return true;
}

// Processes crafting: removes ingredients and adds the crafted item.
bool Crafting::craftRecipe(const CraftingRecipe& recipe, Inventory& inventory) {
    if (!canCraftRecipe(recipe, inventory)) {
        return false;
    }
    // Remove required ingredients.
    for (const auto& ingredient : recipe.ingredients) {
        inventory.removeItem(ingredient.first, ingredient.second);
    }
    // Create the crafted item.
    Texture2D outputTexture = getTextureForItem(recipe.outputType);
    Item craftedItem(recipe.outputType, recipe.outputQuantity, outputTexture);
    inventory.addItem(craftedItem);
    return true;
}

// Convenience: Given an output type, finds the matching recipe and attempts to craft.
bool Crafting::craftItemFromType(ItemType outputType, Inventory& inventory) {
    auto it = std::find_if(recipes.begin(), recipes.end(),
                             [outputType](const CraftingRecipe& r) {
                                 return r.outputType == outputType;
                             });
    if (it == recipes.end()) {
        return false;
    }
    return craftRecipe(*it, inventory);
}

// Renders a crafting UI panel showing available recipes.
// This panel might be shown when the player interacts with a crafting bench.
void Crafting::renderCraftingUI(const Inventory& inventory, const Vector2& uiPosition) {
    float panelWidth = 500;
    float panelHeight = 300;
    // Draw the background panel.
    DrawRectangle(uiPosition.x, uiPosition.y, panelWidth, panelHeight, DARKGRAY);

    int offsetY = 30;
    int recipeIndex = 0;
    for (const auto& recipe : recipes) {
        std::ostringstream oss;
        oss << "Recipe " << (recipeIndex + 1) << ": " << toString(recipe.outputType)
            << " x" << recipe.outputQuantity << " requires ";
        for (const auto& ingredient : recipe.ingredients) {
            oss << toString(ingredient.first) << " x" << ingredient.second << "  ";
        }
        // Draw recipe text.
        DrawText(oss.str().c_str(), uiPosition.x + 10, uiPosition.y + offsetY, 20, WHITE);
        offsetY += 30;
        recipeIndex++;
    }
    // Optionally: add instructions to select/craft a recipe.
    DrawText(("Press number keys 1-" + std::to_string(recipes.size()) + " to craft.").c_str(),
             uiPosition.x + 10, uiPosition.y + panelHeight - 30, 20, LIGHTGRAY);
}

// Helper: Returns a texture for a given item type.
// In production, you'd likely load these once and store them in a resource manager.
Texture2D Crafting::getTextureForItem(ItemType type) {
    switch (type) {
        case ItemType::WOOD:
            return LoadTexture("res/Wood.png");
        case ItemType::STONE:
            return LoadTexture("res/Stone.png");
        case ItemType::CRAFTING_BENCH:
            return LoadTexture("res/Workbench.png");
        case ItemType::CHEST:
            return LoadTexture("res/Chest.png");
        case ItemType::SWORD:
            return LoadTexture("res/Sword.png");
        default:
            return LoadTexture("res/MissingTexture.png");
    }
}
