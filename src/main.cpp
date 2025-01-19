#include <iostream>
#include <cmath>
#include <map>

#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"


int main() {
    InitWindow(480, 360, "2D Adventures Alpha");
    Texture2D text = LoadTexture("../res/icons.png");
    if (text.id == 0) {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }
    SetTextureFilter(text, TEXTURE_FILTER_POINT);
    SetTargetFPS(1000);
    Level level;
    std::vector<std::vector<TileType>> tileMap = level.GetTileMap();
    Player player({360, 320}, tileMap);
    Enemy enemy = { 400, 300 };

    Camera2D camera = { 0 };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update logic
        player.update(deltaTime);
        //enemy.moveTowardPlayer({ player.pos.x, player.pos.y }, deltaTime);

        camera.target = { player.pos.x + 16.0f, player.pos.y + 16.0f };
        camera.target = {roundf(camera.target.x), roundf(camera.target.y)};
        camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 3.0f;

        // Check collision
        /*
        if (isSolid(level.getTileAt(player.pos.x, player.pos.y))) {
            std::cout << "Tile at (" << player.pos.x << ", " << player.pos.y << ") is solid!" << std::endl;
        } else {
            std::cout << "Tile at (" << player.pos.x << ", " << player.pos.y << ") is not solid." << std::endl;
        }
        */

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLACK);

        const int mapWidth = 32, mapHeight = 32;
        const int tileSize = level.tile_size();
        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                TileType tileType = tileMap[y][x];
                Vector2 pos = {
                    static_cast<float>(x * tileSize),
                    static_cast<float>(y * tileSize)
                };

                Rectangle destRect = {
                    pos.x,
                    pos.y,
                    static_cast<float>(tileSize),
                    static_cast<float>(tileSize)
                };

                Rectangle srcRect = {
                    static_cast<float>(static_cast<int>(tileType) * 32),
                    0.0f,
                    32.0f,
                    32.0f
                };
                if (static_cast<int>(tileType) == 4) {
                    DrawTexturePro(text, {0,0,32,32}, destRect, {0, 0}, 0, RAYWHITE);
                }

                DrawTexturePro(text, srcRect, destRect, {0, 0}, 0, RAYWHITE);
            }
        }

        // Draw player
        DrawTexturePro(
            text,
            { player.srcRect.x + player.animationIndex * 32, player.srcRect.y + player.direction * 32, 32, 32 },
            { roundf(player.pos.x), roundf(player.pos.y),32, 32 },
            { 0, 0 },
            0,
            WHITE
        );

        // Draw enemy
        DrawTexturePro(
            text,
            { enemy.srcRect.x + enemy.animationIndex * 32, enemy.srcRect.y + enemy.direction * 32, 32, 32 },
            { enemy.x, enemy.y, 32, 32 },
            { 0, 0 },
            0,
            RED
        );

        EndMode2D();
        DrawFPS(0,0);
        EndDrawing();
    }
    UnloadTexture(text);
    CloseWindow();
}
