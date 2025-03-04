#ifndef CRAFTING_H
#define CRAFTING_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "Inventory.h"
#include "raylib.h"
#include "Stringify.h"

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

    static bool canCraftRecipe(const CraftingRecipe& recipe, const Inventory& inventory) {
        for (const auto& ingredient : recipe.ingredients) {
            if (!inventory.hasItem(ingredient.first, ingredient.second)) {
                return false;
            }
        }
        return true;
    }


    static bool craftRecipe(const CraftingRecipe& recipe, Inventory& inventory) {
        if (!canCraftRecipe(recipe, inventory)) {
            return false;
        }
        // Remove ingredients.
        for (const auto& ingredient : recipe.ingredients) {
            inventory.removeItem(ingredient.first, ingredient.second);
        }
        // Get the texture for the output item.
        Texture2D outputTexture = getTextureForItem(recipe.outputType);
        Item craftedItem(recipe.outputType, recipe.outputQuantity, outputTexture);
        inventory.addItem(craftedItem);
        return true;
    }


    static void updateCraftingUI(Inventory &inventory) {
    if (IsKeyPressed(KEY_ONE)) {
        if (recipes.size() >= 1) {
            bool crafted = craftRecipe(recipes[0], inventory);
            std::string recipeText = "Recipe " + std::to_string(1) + ": " +
                std::string(toString(recipes[0].outputType)) + " x" + std::to_string(recipes[0].outputQuantity) + " requires ";
            for (const auto &ing : recipes[0].ingredients) {
                recipeText += std::string(toString(ing.first)) + " x" + std::to_string(ing.second) + "  ";
            }
            if (crafted) {
                std::cout << "Crafted " << recipes[0].outputQuantity << " of "
                          << std::string(toString(recipes[0].outputType)) << std::endl;
            } else {
                std::cout << "Not enough ingredients for " << std::string(toString(recipes[0].outputType)) << std::endl;
            }
        }
    }
    if (IsKeyPressed(KEY_TWO)) {
        if (recipes.size() >= 2) {
            bool crafted = craftRecipe(recipes[1], inventory);
            std::string recipeText = "Recipe " + std::to_string(2) + ": " +
                std::string(toString(recipes[1].outputType)) + " x" + std::to_string(recipes[1].outputQuantity) + " requires ";
            for (const auto &ing : recipes[1].ingredients) {
                recipeText += std::string(toString(ing.first)) + " x" + std::to_string(ing.second) + "  ";
            }
            if (crafted) {
                std::cout << "Crafted " << recipes[1].outputQuantity << " of "
                          << std::string(toString(recipes[1].outputType)) << std::endl;
            } else {
                std::cout << "Not enough ingredients for "
                          << std::string(toString(recipes[1].outputType)) << std::endl;
            }
        }
    }
    if (IsKeyPressed(KEY_THREE)) {
        if (recipes.size() >= 3) {
            bool crafted = craftRecipe(recipes[2], inventory);
            std::string recipeText = "Recipe " + std::to_string(3) + ": " +
                std::string(toString(recipes[2].outputType)) + " x" + std::to_string(recipes[2].outputQuantity) + " requires ";
            for (const auto &ing : recipes[2].ingredients) {
                recipeText += std::string(toString(ing.first)) + " x" + std::to_string(ing.second) + "  ";
            }
            if (crafted) {
                std::cout << "Crafted " << recipes[2].outputQuantity << " of "
                          << std::string(toString(recipes[2].outputType)) << std::endl;
            } else {
                std::cout << "Not enough ingredients for "
                          << std::string(toString(recipes[2].outputType)) << std::endl;
            }
        }
    }
}

    static void renderCraftingUI(const Inventory &inventory, Vector2 uiPosition) {
        float panelWidth = 500;
        float panelHeight = 300;
        DrawRectangle(uiPosition.x, uiPosition.y, panelWidth, panelHeight, DARKGRAY);

        int offsetY = 30;
        int recipeIndex = 0;
        for (const auto& recipe : recipes) {
            std::string recipeText = "Recipe " + std::to_string(recipeIndex + 1) + ": " +
                std::string(toString(recipe.outputType)) + " x" + std::to_string(recipe.outputQuantity) + " requires ";
            for (const auto& ingredient : recipe.ingredients) {
                recipeText += std::string(toString(ingredient.first)) + " x" + std::to_string(ingredient.second) + "  ";
            }
            DrawText(recipeText.c_str(), uiPosition.x + 10, uiPosition.y + offsetY, 20, WHITE);
            offsetY += 30;
            recipeIndex++;
        }
        DrawText("Press 1-3 to craft", uiPosition.x + 10, uiPosition.y + panelHeight - 30, 20, LIGHTGRAY);
    }

private:
    static Texture2D getTextureForItem(ItemType type) {
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
                return LoadTexture("res/Default.png");
        }
    }
};

#endif // CRAFTING_H

