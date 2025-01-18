#include <iostream>
#include <cmath>
#include <map>

#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"

bool checkCollision(const Rectangle& rect1, const Rectangle& rect2) {
    return CheckCollisionRecs(rect1, rect2);
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
    std::vector<std::vector<int>> tileMap = level.GetTileMap();
    Player player = { 320, 240 };
    Enemy enemy = { 400, 300 };

    Camera2D camera = { 0 };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update logic
        player.update(player, deltaTime);
        enemy.moveTowardPlayer({ player.x, player.y }, deltaTime);

        camera.target = { player.x + 16.0f, player.y + 16.0f };
        camera.target = {roundf(camera.target.x), roundf(camera.target.y)};
        camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 2.0f;

        // Check collision
        bool isColliding = checkCollision(player.getBoundingBox(), enemy.getBoundingBox());

        if (isColliding) {
            //TODO: implement blocking movement
        }

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLACK);

        const int mapWidth = 32, mapHeight = 32;
        const int tileSize = level.tile_size();
        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                int tileType = tileMap[y][x];
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
                    static_cast<float>(tileType * 32),
                    0.0f,
                    32.0f,
                    32.0f
                };
                if (tileType == 4) {
                    DrawTexturePro(text, {0,0,32,32}, destRect, {0, 0}, 0, RAYWHITE);
                }

                DrawTexturePro(text, srcRect, destRect, {0, 0}, 0, RAYWHITE);
            }
        }

        // Draw player
        DrawTexturePro(
            text,
            { player.srcRect.x + player.animationIndex * 32, player.srcRect.y + player.direction * 32, 32, 32 },
            { roundf(player.x), roundf(player.y),32, 32 },
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
        // Draw UI
        if (isColliding) {
            DrawText("Collision Detected!", 20, 80, 20, RED);
        }
        EndDrawing();
    }
    UnloadTexture(text);
    CloseWindow();
}
