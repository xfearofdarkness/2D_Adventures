#include <iostream>
#include <cmath>
#include <map>

#include "Assert.h"
#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"

bool SortEntityByYPos(const Enemy& e1, const Enemy& e2) {
    return e1.y < e2.y;
}
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
    std::vector<Enemy> enemies;
    for (int i = 0; i < 10; i++) {
        auto enemy = Enemy(static_cast<float>(GetRandomValue(0,32*32)), static_cast<float>(GetRandomValue(0,32*32)));
        enemies.push_back(enemy);
    }
    Camera2D camera = { 0 };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update logic
        player.update(deltaTime);

        //enemy.moveTowardPlayer({ player.pos.x, player.pos.y }, deltaTime);
        std::ranges::sort(enemies, SortEntityByYPos);


        for (auto& e : enemies) { //use reference to not copy the whole thing
            if (!e.checkCollision(player.getBoundingBox())) {
                e.moveTowardPlayer(player.pos, deltaTime);
            }
        }


        camera.target = { player.pos.x + 16.0f, player.pos.y + 16.0f };
        camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 2.0f;

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
            { player.pos.x, player.pos.y,32, 32 },
            { 0, 0 },
            0,
            WHITE
        );
        DrawRectangleLinesEx(player.getBoundingBox(), 0.5f, RED);
        if (!enemies.empty()) {
            // Draw enemy
            for (auto & enemy : enemies) {
                DrawTexturePro(
                text,
                { enemy.srcRect.x + enemy.animationIndex * 32, enemy.srcRect.y + enemy.direction * 32, 32, 32 },
                { enemy.x, enemy.y, 32, 32 },
                { 0, 0 },
                0,
                GREEN
            );
                DrawRectangleLinesEx(enemy.getBoundingBox(), 0.5, RED);
            }

        }


        EndMode2D();
        DrawFPS(0,0);
        EndDrawing();
    }
    UnloadTexture(text);
    CloseWindow();
}
