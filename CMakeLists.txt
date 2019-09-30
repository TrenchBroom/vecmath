project(vecmath CXX)

cmake_minimum_required(VERSION 3.12)

set(VECMATH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(vecmath INTERFACE)
target_compile_features(vecmath INTERFACE cxx_std_17)
target_include_directories(vecmath INTERFACE
        $<BUILD_INTERFACE:${VECMATH_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:vecmath/include/vecmath>)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(vecmath INTERFACE -Wall -Wextra -Weverything -pedantic -Wno-c++98-compat -Wno-float-equal)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(vecmath INTERFACE -Wall -Wextra -pedantic)
elseif(MSVC EQUAL 1)
    target_compile_options(vecmath INTERFACE /W3 /EHsc /MP)
else()
    message(FATAL_ERROR "Cannot set compile options for target common")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(lib)
add_subdirectory(test)
