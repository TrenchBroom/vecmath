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
    #  -Wno-format -Wno-variadic-macros -Wno-c99-extensions -Wno-padded -Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -Wno-weak-template-vtables -Wno-float-equal -Wno-used-but-marked-unused -Wno-format-nonliteral -Wno-missing-noreturn -Wno-unused-local-typedef -Wno-double-promotion -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-implicit-fallthrough -Wno-zero-as-null-pointer-constant -Wno-switch-enum
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(vecmath INTERFACE -Wall -Wextra -pedantic)
    #  -Wno-format -Wno-variadic-macros -Wno-padded -Wno-unused-parameter -Wno-float-equal -Wno-format-nonliteral -Wno-missing-noreturn -Wno-zero-as-null-pointer-constant -Wno-error=maybe-uninitialized
elseif(MSVC EQUAL 1)
    target_compile_options(vecmath INTERFACE /W3 /EHsc /MP)
else()
    message(FATAL_ERROR "Cannot set compile options for target common")
endif()


add_subdirectory(lib)
add_subdirectory(test)
