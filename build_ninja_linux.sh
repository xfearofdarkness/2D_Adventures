#!/bin/bash

set -e

BUILD_DIR="build"

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo "CMake is not installed. Please install it first."
    exit 1
fi

# Check for Ninja
if ! command -v ninja &> /dev/null; then
    echo "Fatal: Ninja is not installed."
    exit 1
fi

# Create a new build directory and configure with Ninja
echo "Configuring project with Ninja..."
cmake -G Ninja -S . -B "$BUILD_DIR"


echo "Building the game..."
cmake --build "$BUILD_DIR"


echo "Done!"