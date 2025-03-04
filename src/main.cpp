#include <iostream>
#include <cmath>
#include <map>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "Assert.h"
#include "DroppedItem.h"
#include "EndBoss.h"
#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"
#include "raymath.h"
#include "Slime.h"


enum class GameState {
    MENU,
    HELP,
    GAMING,
    GAMEOVER,
    PAUSED,
    NEXTLVL,
    WON
};



bool SortEntityByYPos(const Enemy& e1, const Enemy& e2) {
    return e1.pos.y < e2.pos.y;
}

void SpawnEnemies(Level &level, std::vector<Enemy> &enemies, int amount, Vector2 playerPos) {
    for (int i = 0; i < amount; i++) {
        bool spawned = false;
        for (int attempts = 0; attempts < amount*3; attempts++) {
            int tileX = GetRandomValue(0, 32 - 1);
            int tileY = GetRandomValue(0, 32 - 1);
            float dist = Vector2Distance({static_cast<float>(tileX*32), static_cast<float>(tileY*32)}, playerPos);
            if (!isSolid(level.getTileAt(tileX*32, tileY*32)) && dist > 300) {

                float x = static_cast<float>(tileX) * 32.0f;
                float y = static_cast<float>(tileY) * 32.0f;

                enemies.push_back(Enemy({x, y}, level));
                spawned = true;
                break;
            }
        }

        if (!spawned) {
            std::cout << "No spawn point for " << i << " was found.\n";
        }
    }
}

void SpawnSlimes(Level &level, std::vector<Slime> &enemies, int amount, Vector2 playerPos) {
    for (int i = 0; i < amount; i++) {
        bool spawned = false;
        for (int attempts = 0; attempts < amount*3; attempts++) {
            int tileX = GetRandomValue(0, 32 - 1);
            int tileY = GetRandomValue(0, 32 - 1);
            float dist = Vector2Distance({static_cast<float>(tileX*32), static_cast<float>(tileY*32)}, playerPos);
            if (!isSolid(level.getTileAt(tileX*32, tileY*32)) && dist > 300) {

                float x = static_cast<float>(tileX) * 32.0f;
                float y = static_cast<float>(tileY) * 32.0f;

                enemies.push_back(Slime({x, y}, level));
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
                static_cast<float>(x * 32),
                static_cast<float>(y * 32)
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

void ResetGame(Player& player, std::vector<std::vector<TileType>> &tileMap, std::vector<Enemy> &enemies, std::vector<Slime> slimes,
               Level &level, RenderTexture2D &tileMapTexture, Texture2D tileAtlas, GameState &state, bool keepInv) {
    player.reset(keepInv);
    if (state==GameState::GAMEOVER) level.reload(1);

    if (state==GameState::NEXTLVL) {
        level.reload(2);
        level.setLevel(2);
    }
    // Clear enemy list
    enemies.clear();
    slimes.clear();
    // Free old tilemap texture before reloading
    UnloadRenderTexture(tileMapTexture);

    tileMap = level.GetTileMap();
    tileMapTexture = CreateBackgroundRenderTexture(tileMap, tileAtlas, level, 32, 32);

    state = GameState::GAMING;
}

int main() {
    std::vector<DroppedItem> droppedItems;
    auto state = GameState::MENU;

    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(1280,720, "2D Adventures Alpha");
    Texture2D gameIcon = LoadTexture("res/Icon.png");
    Texture2D tileAtlas = LoadTexture("res/icons.png");
    SetTextureFilter(tileAtlas, TEXTURE_FILTER_POINT);
    if (tileAtlas.id == 0) {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }
    const int hz = GetMonitorRefreshRate(GetCurrentMonitor());
    std::cout << "Monitor refresh rate: " << hz << std::endl;
    SetTargetFPS(hz);
    SetExitKey(0);

    std::vector<Enemy> enemies;
    std::vector<Slime> slimes;
    Level level{1};
    std::vector<std::vector<TileType>> tileMap = level.GetTileMap();
    Player player({14*32, 3*32}, level, &droppedItems);
    EndBoss boss({15*32,30*32}, level);

    int spawnAmount = 10;
    enemies.reserve(spawnAmount*2);
    slimes.reserve(spawnAmount*2);
    SpawnEnemies(level, enemies, spawnAmount, player.pos);
    SpawnSlimes(level, slimes, spawnAmount, player.pos);
    Camera2D camera = { 0 };
    RenderTexture2D tilemapTexture = CreateBackgroundRenderTexture(tileMap, tileAtlas, level, 32, 32);
    SetTextureWrap(tileAtlas, TEXTURE_WRAP_CLAMP);

    player.initInventory();


    ClearWindowState(FLAG_WINDOW_HIDDEN); //Unhide window after initialization

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_I)) {
            state = GameState::WON;
        }
        if (!boss.isAlive) state = GameState::WON;
        if (state == GameState::WON) {
            if (int quit = GuiButton({ static_cast<float>(GetScreenWidth()/2-100), static_cast<float>(GetScreenHeight()/2)-40, 200, 40 }, "Quit")) {
                break;
            }
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawText("YOU WIN", GetScreenWidth()/2.0f-100, GetScreenHeight()/2-300, 40, GOLD);
            DrawText("Thanks for playing", GetScreenWidth()/2.0f-200, GetScreenHeight()/2-200, 40, BLACK);
            EndDrawing();
        }

        if (GetKeyPressed() == KEY_ESCAPE) {
            state = GameState::MENU;
        }

        if (state == GameState::NEXTLVL) {
            if (int next = GuiButton({ static_cast<float>(GetScreenWidth())/2.0f-100, static_cast<float>(GetScreenHeight())/2-100, 200, 40 }, "Continue")) {
                ResetGame(player, tileMap, enemies, slimes, level, tilemapTexture, tileAtlas, state, true);
                state = GameState::GAMING;
            }
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawText("LEVEL COMPLETED", (GetScreenWidth()/2-250), (GetScreenHeight()/2-200), 50, BLUE);
            EndDrawing();
        }
        if (state == GameState::MENU) {
            guiFont.baseSize = 5;
            if (int start = GuiButton({ static_cast<float>(GetScreenWidth())/2.0f-100, static_cast<float>(GetScreenHeight())/2-100, 200, 40 }, "Start Game")) {
                state = GameState::GAMING;
            }
            if (int quit = GuiButton({static_cast<float>(GetScreenWidth())/2-100,static_cast<float>(GetScreenHeight())/2-50,200,40}, "Quit Game")) {
                break;
            }
            if (int help = GuiButton({static_cast<float>(GetScreenWidth())/2-100,static_cast<float>(GetScreenHeight())/2,200,40}, "Help and Controls")) {
                state = GameState::HELP;
            }
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawText("MAIN MENU", (GetScreenWidth()/2-150), (GetScreenHeight()/2-200), 50, BLACK);
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

        if (player.state == PlayerState::DEAD) {
            state = GameState::GAMEOVER;
        }

        if (state == GameState::GAMEOVER) {
            guiFont.baseSize = 8;
            if (int retry = GuiButton({ static_cast<float>(GetScreenWidth()/2-100), static_cast<float>(GetScreenHeight()/2)-100, 200, 40 }, "Retry")) {
                ResetGame(player, tileMap, enemies, slimes, level, tilemapTexture, tileAtlas, state, false);
                droppedItems.clear();
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

        if (state == GameState::GAMING || state == GameState::PAUSED) {
            // Update logic
            player.update(deltaTime, enemies, slimes, boss);
            if (player.checkLevelTransition()) state = GameState::NEXTLVL;
            if (state != GameState::PAUSED && !player.getPaused()) {
                if (level.needsRefreshing) {
                    UnloadRenderTexture(tilemapTexture);
                    tilemapTexture = CreateBackgroundRenderTexture(level.GetTileMap(), tileAtlas, level, 32, 32);
                }


                //enemy.moveTowardPlayer({ player.pos.x, player.pos.y }, deltaTime);
                if (!enemies.empty()) {
                    std::ranges::sort(enemies, SortEntityByYPos);
                }
                if (!slimes.empty()) {
                    for (auto &slime : slimes) {
                        if (!slime.checkCollisionWithPlayer(player.getBoundingBox())) {
                            slime.update(player, deltaTime);
                        } else {
                            slime.attack(player, 1);
                            slime.setState(EnemyState::WANDERING);
                        }
                    }
                }
                if (!enemies.empty()) {
                    for (auto& e : enemies) { //use reference to not copy the whole thing
                        if (!e.checkCollisionWithPlayer(player.getBoundingBox())) {
                            e.update(player, deltaTime);
                        } else {
                            e.attack(player, 1);
                            e.setState(EnemyState::WANDERING);
                        }
                    }
                }
                std::erase_if(enemies, [enemies](auto& e) {
                    return !e.isAlive && !enemies.empty();
                });
                std::erase_if(slimes, [slimes](auto& slime) {
                    return !slime.isAlive && !slimes.empty();
                });


                if (enemies.empty()) {
                    SpawnEnemies(level, enemies, spawnAmount*level.getLevel(), player.pos);
                }
                if (level.getLevel() ==2) {
                    boss.update(player, deltaTime);
                    if (CheckCollisionRecs(player.getBoundingBox(), boss.getBoundingBox())) {
                        boss.attack(player, boss.getDamage());
                        boss.setState(EnemyState::WANDERING);
                    }
                }
                camera.target = { player.pos.x + 16.0f, player.pos.y + 16.0f };
                camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
                camera.rotation = 0.0f;
                camera.zoom = 3.0f;
            }



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

            for (const auto& droppedItem : droppedItems) {
                droppedItem.render();
            }

            // Draw player
            DrawTexturePro(
                tileAtlas,
                { player.srcRect.x + player.animationIndex * 32, player.srcRect.y + player.direction * 32, 32, 32 },
                { player.pos.x, player.pos.y,32, 32 },
                { 0, 0 },
                0,
                WHITE
            );
            player.renderAttack(tileAtlas);
            //DrawRectangleLinesEx(player.getBoundingBox(), 0.5f, RED);

            if (!enemies.empty()) {
                // Draw enemy
                for (auto & enemy : enemies) {
                    DrawTexturePro(
                    tileAtlas,
                    { enemy.srcRect.x + enemy.animationIndex * 32, enemy.srcRect.y + enemy.direction * 32, 32, 32 },
                    { enemy.pos.x, enemy.pos.y, 32, 32 },
                    { 0, 0 },
                    0,
                    GREEN
                );
                    //DrawRectangleLinesEx(enemy.getBoundingBox(), 0.5, RED);
                }

            }

            if (!slimes.empty()) {
                for (auto& slime : slimes) {
                    slime.render(tileAtlas);
                }
            }

            if (level.getLevel() == 2) boss.render(tileAtlas);

            EndTextureMode();

            EndMode2D();
            DrawFPS(0,0);
            player.renderInventory();
            player.renderHUD();
            EndDrawing();
        }
    }
    UnloadTexture(tileAtlas);
    UnloadRenderTexture(tilemapTexture);
    CloseWindow();
}
