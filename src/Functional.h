//
// Created by jamie on 04.03.2025.
//
#pragma once
#include <functional>  // For std::hash

#include "raylib.h"

struct Vector2Hash {
    std::size_t operator()(const Vector2& v) const noexcept {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        return h1 ^ (h2 << 1);  // XOR + shift to mix hashes
    }
};
