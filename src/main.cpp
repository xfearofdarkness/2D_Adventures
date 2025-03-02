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

enum class GameState {
    MENU,
    HELP,
    GAMING,
    GAMEOVER
};



bool SortEntityByYPos(const Enemy& e1, const Enemy& e2) {
    return e1.pos.y < e2.pos.y;
}

void SpawnEnemies(std::vector<std::vector<TileType>> &tileMap, const Level &level, std::vector<Enemy> &enemies, int amount) {
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

RenderTexture2D CreateBackgroundRenderTexture(const std::vector<std::vector<TileType>>& tilemap,Texture2D tileAtlas, Level &level, int width, int height) {
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
    
    level.needsRefreshing = false;

    return renderTexture;
}

void ResetGame(Player& player, std::vector<std::vector<TileType>> &tileMap, std::vector<Enemy> &enemies, Level &level, RenderTexture2D &tileMapTexture, Texture2D tileAtlas, GameState &state) {
    player.reset();
    level.reload();
    tileMap = level.GetTileMap();
    enemies.clear();
    tileMapTexture = CreateBackgroundRenderTexture(tileMap, tileAtlas, level, 32, 32);
    state = GameState::GAMING;
}

int main() {
    GameState state = GameState::MENU;
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(920, 720, "2D Adventures Alpha");

    Texture2D text = LoadTexture("res/icons.png");
    SetTextureFilter(text, TEXTURE_FILTER_POINT);
    if (text.id == 0) {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }
    const int hz = GetMonitorRefreshRate(GetCurrentMonitor());
    std::cout << "Monitor refresh rate: " << hz << std::endl;
    SetTargetFPS(hz);
    SetExitKey(0);
    std::vector<Enemy> enemies;
    Level level;
    std::vector<std::vector<TileType>> tileMap = level.GetTileMap();

    int spawnAmount = 10;
    enemies.reserve(spawnAmount);
    SpawnEnemies(tileMap, level, enemies, spawnAmount);
    Camera2D camera = { 0 };
    RenderTexture2D tilemapTexture = CreateBackgroundRenderTexture(tileMap, text, level, 32, 32);
    SetTextureWrap(text, TEXTURE_WRAP_CLAMP);
    Player player({360, 320}, level);
    player.initInventory();


    ClearWindowState(FLAG_WINDOW_HIDDEN); //unhide window after initialization
    while (!WindowShouldClose()) {
        if (GetKeyPressed() == KEY_ESCAPE) {
            state = GameState::MENU;
        }
        if (state == GameState::MENU) {
            guiFont.baseSize = 5;
            if (int start = GuiButton({ static_cast<float>(GetScreenWidth()/2-100), static_cast<float>(GetScreenHeight()/2)-100, 200, 40 }, "Start Game")) {
                state = GameState::GAMING;
            }
            if (int quit = GuiButton({static_cast<float>(GetScreenWidth()/2-100),static_cast<float>(GetScreenHeight()/2)-50,200,40}, "Quit Game")) {
                break;
            }
            if (int help = GuiButton({static_cast<float>(GetScreenWidth()/2-100),static_cast<float>(GetScreenHeight()/2),200,40}, "Help and Controls")) {
                state = GameState::HELP;
            }
            BeginDrawing();
            ClearBackground(SKYBLUE);
            guiFont.baseSize = 3;
            GuiLabel({ static_cast<float>(GetScreenWidth()/2)-200, 0, 200, 100 }, "MAIN MENU");
            guiFont.baseSize = 5;
            EndDrawing();
        }

        if (state == GameState::HELP) {
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
                state = GameState::MENU;
            }
            EndDrawing();
        }

        float deltaTime = GetFrameTime();

        if (IsKeyDown(KEY_K)) {
            player.state = PlayerState::DEAD;
        }

        if (player.state == PlayerState::DEAD) {
            state = GameState::GAMEOVER;
        }

        if (state == GameState::GAMEOVER) {
            guiFont.baseSize = 8;
            if (int retry = GuiButton({ static_cast<float>(GetScreenWidth()/2-100), static_cast<float>(GetScreenHeight()/2)-100, 200, 40 }, "Retry")) {
                ResetGame(player, tileMap, enemies, level, tilemapTexture, text, state);
            }

            if (int quit = GuiButton({ static_cast<float>(GetScreenWidth()/2-100), static_cast<float>(GetScreenHeight()/2)-40, 200, 40 }, "Quit")) {
                break;
            }
            BeginDrawing();
            ClearBackground(GRAY);

            DrawText("Game Over!", static_cast<float>(GetScreenWidth()/2-250), 10, 100, RED);
            guiFont.baseSize = 5;
            EndDrawing();
        }

        if (state == GameState::GAMING) {
            // Update logic
            player.update(deltaTime, enemies);

            if (level.needsRefreshing) {
                UnloadRenderTexture(tilemapTexture);
                tilemapTexture = CreateBackgroundRenderTexture(level.GetTileMap(), text, level, 32, 32);
            }


            //enemy.moveTowardPlayer({ player.pos.x, player.pos.y }, deltaTime);
            if (!enemies.empty()) {
                std::ranges::sort(enemies, SortEntityByYPos);
            }
            if (!enemies.empty()) {
                for (auto& e : enemies) { //use reference to not copy the whole thing
                    if (!e.checkCollisionWithPlayer(player.getBoundingBox())) {
                        e.update(player.pos, deltaTime);
                    } else {
                        e.attack(player, 1);
                        e.setState(EnemyState::WANDERING);
                    }
                }
            }
            std::erase_if(enemies, [enemies](auto& e) {
                return !e.isAlive && !enemies.empty();
            });

            if (enemies.empty()) {
                SpawnEnemies(tileMap, level, enemies, spawnAmount);
            } else if (static_cast<int>(enemies.size()) <= spawnAmount / 2) {
                SpawnEnemies(tileMap, level, enemies, std::clamp(spawnAmount-static_cast<int>(enemies.size()), 1, 10));
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
            if (player.state == PlayerState::ATTACKING) DrawRectangleLines((int)player.attackBoxRec.x, (int)player.attackBoxRec.y, (int)player.attackBoxRec.width, (int)player.attackBoxRec.height, YELLOW);
            if (player.state == PlayerState::ATTACKING) DrawTexturePro(text, { player.attackSrcRect.x + player.direction * 32 , player.attackSrcRect.y, 32, 32}, player.attackBoxRec, { 0,0 }, 0, WHITE);
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
            player.renderInventory();
            player.renderHUD();
            EndDrawing();
        }
    }
    UnloadTexture(text);
    UnloadRenderTexture(tilemapTexture);
    CloseWindow();
}
