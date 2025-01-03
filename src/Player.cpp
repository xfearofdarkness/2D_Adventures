//
// Created by jamie on 03.01.2025.
//

#include "Player.h"



void Player::update(Player &player, float deltaTime) {
    move(player, deltaTime);
}

void move(Player& player, float deltaTime) {
    bool up = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool down = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    bool left = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    float speed = 100.0f; // Speed in pixels per second
    bool moved = false;
    bool shouldMove = true;

    if (up && down) {
        shouldMove = false;
    }
    if (left && right) {
        shouldMove = false;
    }
    if (shouldMove) {
        if (up) {
            player.y -= speed * deltaTime;
            player.direction = 1; // Up
            moved = true;
        }
        if (down) {
            player.y += speed * deltaTime;
            player.direction = 0; // Down
            moved = true;
        }
        if (left) {
            player.x -= speed * deltaTime;
            player.direction = 2; // Left
            moved = true;
        }
        if (right) {
            player.x += speed * deltaTime;
            player.direction = 3; // Right
            moved = true;
        }
    }



    // Animation logic
    if (moved) {
        player.stepCount += speed * deltaTime;

        if (player.stepCount > 16.0f) { // Change frame every 32 pixels
            player.stepCount = 0.0f;
            player.animationIndex = (player.animationIndex + 1) % 2; // 2 frames per direction
        }
    }
}