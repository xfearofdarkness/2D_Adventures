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

        int mapWidth = 32, mapHeight = 32;
        int tileSize = level.tile_size();
        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                int tileType = tileMap[y][x];
                Vector2 pos = { (float)(x * tileSize),(float)(y * tileSize) };
                switch (tileType) {
                    case 0:
                        DrawTexturePro(text, { 32*0,0, 32, 32 }, {pos.x, pos.y, (float)tileSize, (float)tileSize}, {0,0}, 0, RAYWHITE);
                        break;
                    case 1:
                        DrawTexturePro(text, { 32*1,0, 32, 32 }, {pos.x, pos.y, (float)tileSize, (float)tileSize}, {0,0}, 0, RAYWHITE);
                        break;
                    case 2:
                        DrawTexturePro(text, { 32*2,0, 32, 32 }, {pos.x, pos.y, (float)tileSize, (float)tileSize}, {0,0}, 0, RAYWHITE);
                        break;
                    case 3:
                        DrawTexturePro(text, { 32*3,0, 32, 32 }, {pos.x, pos.y, (float)tileSize, (float)tileSize}, {0,0}, 0, RAYWHITE);
                        break;
                    case 4:
                        DrawTexturePro(text, { 32*0,0, 32, 32 }, {pos.x, pos.y, (float)tileSize, (float)tileSize}, {0,0}, 0, RAYWHITE);
                        DrawTexturePro(text, { 32*4,0, 32, 32 }, {pos.x, pos.y, (float)tileSize, (float)tileSize}, {0,0}, 0, RAYWHITE);
                        break;
                    default:
                        break;
                }
            }
        }

        // Draw player
        DrawTexturePro(
            text,
            { player.srcRect.x + player.animationIndex * 32, player.srcRect.y + player.direction * 32, 32, 32 },
            { camera.target.x-16, camera.target.y - 16,32, 32 },
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
