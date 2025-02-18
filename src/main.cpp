#include <iostream>
#include <cmath>
#include <map>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "Assert.h"
#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"


const Level level;
std::vector<std::vector<TileType>> tileMap = level.GetTileMap();
Player player({360, 320}, tileMap);
std::vector<Enemy> enemies;
bool SortEntityByYPos(const Enemy& e1, const Enemy& e2) {
    return e1.pos.y < e2.pos.y;
}

void SpawnEnemies(std::vector<std::vector<TileType>>& tileMap, std::vector<Enemy>& enemies, int amount) {
    for (int i = 0; i < amount; i++) {
        bool spawned = false;
        for (int attempts = 0; attempts < amount*3; attempts++) {
            int tileX = GetRandomValue(0, 32 - 1);
            int tileY = GetRandomValue(0, 32 - 1);

            if (!isSolid(level.getTileAt(tileX*32, tileY*32))) {

                float x = tileX * 32.0f;
                float y = tileY * 32.0f;

                enemies.push_back(Enemy({x, y}, tileMap));
                spawned = true;
                break;
            }
        }

        if (!spawned) {
            std::cout << "No spawn point for " << i << " was found.\n";
        }
    }
}
RenderTexture2D CreateBackgroundRenderTexture(const std::vector<std::vector<TileType>>& tilemap,Texture2D tileAtlas, int width, int height) {
    int mapWidth = static_cast<int>(tilemap[0].size());
    int mapHeight = static_cast<int>(tilemap.size());

    RenderTexture2D renderTexture = LoadRenderTexture(width*32, height*32);

    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            TileType tile = tilemap[y][x];

            Vector2 pos = {
                roundf(static_cast<float>(x * 32)),
                roundf(static_cast<float>(y * 32))
            };


            Rectangle srcRect = {
                static_cast<float>(static_cast<int>(tile) * 32),
                0.0f,
                32.0f,
                32.0f
            };

            DrawTextureRec(tileAtlas, srcRect, pos, RAYWHITE);
        }
    }

    EndTextureMode();

    return renderTexture;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(480, 360, "2D Adventures Alpha");

    Texture2D text = LoadTexture("../res/icons.png");
    SetTextureFilter(text, TEXTURE_FILTER_POINT);
    if (text.id == 0) {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }
    const int hz = GetMonitorRefreshRate(GetCurrentMonitor());
    std::cout << "Monitor refresh rate: " << hz << std::endl;
    SetTargetFPS(hz);
    SetExitKey(0);
    int spawnAmount = 10;
    enemies.reserve(spawnAmount);
    SpawnEnemies(tileMap, enemies, spawnAmount);
    Camera2D camera = { 0 };
    RenderTexture2D tilemapTexture = CreateBackgroundRenderTexture(tileMap, text, 32, 32);
    SetTextureWrap(text, TEXTURE_WRAP_CLAMP);
    bool showMenu = true;
    bool startGame = false;
    bool showHelp = false;

    ClearWindowState(FLAG_WINDOW_HIDDEN); //unhide window after initialization
    while (!WindowShouldClose()) {
        if (GetKeyPressed() == KEY_ESCAPE) {
            startGame = !startGame;
            showMenu = !showMenu;
        }
        if (showMenu) {
            guiFont.baseSize = 5;
            if (int start = GuiButton({ static_cast<float>(GetScreenWidth()/2-100), 100, 200, 40 }, "Start Game")) {
                startGame = true;
                showMenu = false;
            }
            if (int quit = GuiButton({static_cast<float>(GetScreenWidth()/2-100),200.0f,200,40}, "Quit Game")) {
                break;
            }
            if (int help = GuiButton({static_cast<float>(GetScreenWidth()/2-100),150.0f,200,40}, "Help and Controls")) {
                showHelp = true;
                showMenu = false;
            }
            BeginDrawing();
            ClearBackground(SKYBLUE);
            guiFont.baseSize = 3;
            GuiLabel({ 150, 0, 200, 100 }, "MAIN MENU");
            guiFont.baseSize = 5;
            EndDrawing();
        }

        if (showHelp) {
            BeginDrawing();
            ClearBackground(SKYBLUE);

            int done = GuiMessageBox({0,0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},"Controls" , "", "OK");

            guiFont.baseSize = 3;
            GuiLabel({ 60, 80, 400, 20 }, "Move with WASD");
            GuiLabel({ 60, 110, 400, 20 }, "Hit with C or CTRL");
            GuiLabel({ 60, 140, 400, 20 }, "Press ESC to pause.");
            guiFont.baseSize = 5;
            GuiLabel({ 60, 190, 400, 30 }, "A Game for a school CS");
            GuiLabel({ 60, 230, 400, 30 }, "project by Jamie Huta");
            if (done == 1 || done == 0) {
                showHelp = false;
                showMenu = true;
            }
            EndDrawing();
        }

        float deltaTime = GetFrameTime();
        if (startGame) {
            // Update logic
            player.update(deltaTime, enemies);


            //enemy.moveTowardPlayer({ player.pos.x, player.pos.y }, deltaTime);
            if (!enemies.empty()) {
                std::ranges::sort(enemies, SortEntityByYPos);
            }
            if (!enemies.empty()) {
                for (auto& e : enemies) { //use reference to not copy the whole thing
                    if (!e.checkCollisionWithPlayer(player.getBoundingBox())) {
                        e.update(player.pos, deltaTime);
                    }
                }
            }
            std::erase_if(enemies, [](auto& e) {
                return !e.isAlive && !enemies.empty();
            });

            if (enemies.empty()) {
                SpawnEnemies(tileMap, enemies, spawnAmount);
            } else if (static_cast<int>(enemies.size()) <= spawnAmount / 2) {
                SpawnEnemies(tileMap, enemies, std::clamp(spawnAmount-static_cast<int>(enemies.size()), 1, 10));
            }

            camera.target = { player.pos.x + 16.0f, player.pos.y + 16.0f };
            camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
            camera.rotation = 0.0f;
            camera.zoom = 2.0f;


            BeginDrawing();
            BeginMode2D(camera);
            ClearBackground(BLANK);

            //Draw tile map
            DrawTexturePro(
               tilemapTexture.texture,
               Rectangle{ 0, 0, (float)tilemapTexture.texture.width, (float)-tilemapTexture.texture.height }, // Flip vertically
               Rectangle{ 0, 0, (float)tilemapTexture.texture.width, (float)tilemapTexture.texture.height },
               Vector2{ 0, 0 },
               0.0f,
               WHITE
           );

            // Draw player
            DrawTexturePro(
                text,
                { player.srcRect.x + player.animationIndex * 32, player.srcRect.y + player.direction * 32, 32, 32 },
                { player.pos.x, player.pos.y,32, 32 },
                { 0, 0 },
                0,
                WHITE
            );
            DrawRectangleLines((int)player.attackBoxRec.x, (int)player.attackBoxRec.y, (int)player.attackBoxRec.width, (int)player.attackBoxRec.height, YELLOW);
            DrawRectangleLinesEx(player.getBoundingBox(), 0.5f, RED);
            if (!enemies.empty()) {
                // Draw enemy
                for (auto & enemy : enemies) {
                    DrawTexturePro(
                    text,
                    { enemy.srcRect.x + enemy.animationIndex * 32, enemy.srcRect.y + enemy.direction * 32, 32, 32 },
                    { enemy.pos.x, enemy.pos.y, 32, 32 },
                    { 0, 0 },
                    0,
                    GREEN
                );
                    DrawRectangleLinesEx(enemy.getBoundingBox(), 0.5, RED);
                }

            }
            EndTextureMode();

            EndMode2D();
            DrawFPS(0,0);
            EndDrawing();
        }
    }
    UnloadTexture(text);
    UnloadRenderTexture(tilemapTexture);
    CloseWindow();
}
