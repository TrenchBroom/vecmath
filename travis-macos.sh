#!/bin/bash

set -o verbose

brew update
brew install cmake cppcheck ninja

# Sometimes homebrew complains that cmake is already installed, but we need the latest version.
brew upgrade cmake

# Check versions
cmake --version
cppcheck --version

# Build TB

mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_FLAGS="-Werror" || exit 1

# cmake --build . --target cppcheck
# if [[ $? -ne 0 ]] ; then
#    echo
#    echo "cppcheck detected issues, see below"
#    echo
#
#    cat cppcheck-errors.txt
#    echo
#
#    exit 1
#fi

cmake --build . --config "Release" || exit 1

BUILD_DIR=$(pwd)

cd "$BUILD_DIR/test/"
./vecmath-test || exit 1
cd "$BUILD_DIR"
