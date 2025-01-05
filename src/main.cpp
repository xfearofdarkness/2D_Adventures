#include <iostream>
#include <cmath>

#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "icons.h"

bool checkCollision(const Rectangle& rect1, const Rectangle& rect2) {
    return CheckCollisionRecs(rect1, rect2);
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(640, 480, "2D Adventures Alpha");
    Image img = LoadImageFromMemory(".png", icon_png, icon_png_len);
    Texture2D text = LoadTextureFromImage(img);
    SetTextureFilter(text, TEXTURE_FILTER_POINT);
    SetTargetFPS(60);

    Player player = { 320, 240 };
    Enemy enemy = { 400, 300 };

    Camera2D camera = { 0 };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update logic
        player.update(player, deltaTime);
        enemy.moveTowardPlayer({ player.x, player.y }, deltaTime);

        camera.target = { roundf(player.x + 16.0f), roundf(player.y + 16.0f) };
        camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        // Check collision
        bool isColliding = checkCollision(player.getBoundingBox(), enemy.getBoundingBox());

        if (isColliding) {
            //TODO: implement blocking movement
        }

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLACK);


        for (int y = -32 * 10; y < 32 * 20; y += 32) {
            for (int x = -32 * 10; x < 32 * 20; x += 32) {
                DrawTexturePro(text, { 0, 0, 32, 32 }, { (float)x, (float)y, 32, 32 }, { 0, 0 }, 0, WHITE);
            }
        }

        // Draw player
        DrawTexturePro(
            text,
            { player.srcRect.x + player.animationIndex * 32, player.srcRect.y + player.direction * 32, 32, 32 },
            { roundf(player.x), roundf(player.y), 32, 32 },
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
        // Draw UI
        if (isColliding) {
            DrawText("Collision Detected!", 20, 80, 20, RED);
        }
        EndDrawing();
    }
    UnloadTexture(text);
    UnloadImage(img);
    CloseWindow();
}
