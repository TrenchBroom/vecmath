#!/bin/bash

set -o verbose

brew update
brew install cmake ninja

# Sometimes homebrew complains that cmake is already installed, but we need the latest version.
brew upgrade cmake

# Check versions
cmake --version

# Build TB

mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_FLAGS="-Werror" || exit 1

cmake --build . --config "Release" || exit 1

BUILD_DIR=$(pwd)

cd "$BUILD_DIR/test/"
./vecmath-test || exit 1
cd "$BUILD_DIR"
