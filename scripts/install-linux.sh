#!/bin/bash

# Set project root directory
PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
BUILD_DIR="$PROJECT_ROOT/build"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit

cmake ..
cmake --build . --config Release
cmake --install .
