#!/bin/bash

set -o verbose

if [[ $TB_GCC8 == "true" ]] ; then
    export CC=gcc-8
    export CXX=g++-8
else
    export CC=gcc-7
    export CXX=g++-7
fi

# Check versions
cmake --version

# Ubuntu's cppcheck is too old
#cppcheck --version

# Build libvecmath

mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-Werror || exit 1
# Ubuntu's cppcheck is too old
#cmake --build . --target cppcheck || exit 1
cmake --build . --config Release || exit 1

# Run tests

BUILD_DIR=$(pwd)

cd "$BUILD_DIR/test"
./vecmath-test || exit 1
cd "$BUILD_DIR"
