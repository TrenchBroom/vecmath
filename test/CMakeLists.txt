add_executable(vecmath-test)
target_sources(vecmath-test PRIVATE
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/bbox_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/convex_hull_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/distance_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/intersection_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/line_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/mat_ext_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/mat_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/plane_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/polygon_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/quat_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/ray_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/scalar_test.cpp"
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/segment_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/vec_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/run_all.cpp"
        )

target_link_libraries(vecmath-test gtest vecmath)
