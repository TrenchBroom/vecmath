add_executable(vecmath-test)
target_sources(vecmath-test PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/bbox_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/convex_hull_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/distance_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/intersection_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/line_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/mat_ext_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/mat_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/plane_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/polygon_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/quat_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ray_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/scalar_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/segment_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/vec_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/run_all.cpp"
        )

target_link_libraries(vecmath-test gtest vecmath)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(vecmath-test PRIVATE -Wall -Wextra -Weverything -pedantic -Wno-c++98-compat -Wno-global-constructors -Wno-zero-as-null-pointer-constant)
    #  -Wno-format -Wno-variadic-macros -Wno-c99-extensions -Wno-padded -Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -Wno-weak-template-vtables -Wno-float-equal -Wno-used-but-marked-unused -Wno-format-nonliteral -Wno-missing-noreturn -Wno-unused-local-typedef -Wno-double-promotion -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-implicit-fallthrough -Wno-zero-as-null-pointer-constant -Wno-switch-enum
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(vecmath-test PRIVATE -Wall -Wextra -pedantic)
    #  -Wno-format -Wno-variadic-macros -Wno-padded -Wno-unused-parameter -Wno-float-equal -Wno-format-nonliteral -Wno-missing-noreturn -Wno-zero-as-null-pointer-constant -Wno-error=maybe-uninitialized
elseif(MSVC EQUAL 1)
    target_compile_options(vecmath-test PRIVATE /W3 /EHsc /MP)
else()
    message(FATAL_ERROR "Cannot set compile options for target common")
endif()
