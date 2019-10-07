/*
 Copyright (C) 2010-2019 Kristian Duske

 This file is part of libvecmath.

 libvecmath is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 libvecmath is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with libvecmath. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include "test_utils.h"

#include <vecmath/abstract_line.h>
#include <vecmath/forward.h>
#include <vecmath/vec.h>
#include <vecmath/vec_ext.h>
#include <vecmath/quat.h>
#include <vecmath/constexpr_util.h>
#include <vecmath/intersection.h>

#include <array>

namespace vm {
    bool lineOnPlane(const plane3f& plane, const line3f& line);

    template <typename C>
    constexpr bool containsPoint(const C& vertices, const vec3d& point) {
        return polygon_contains_point(point, vec3d::pos_z(), std::begin(vertices), std::end(vertices));
    }

    constexpr std::array<vec3d, 4> square() {
        return {
            vec3d(-1.0, -1.0, 0.0),
            vec3d(-1.0, +1.0, 0.0),
            vec3d(+1.0, +1.0, 0.0),
            vec3d(+1.0, -1.0, 0.0)
        };
    }

    constexpr std::array<vec3d, 3> triangle() {
        return {
            vec3d(-1.0, +1.0, 0.0), // top
            vec3d(-1.0, -1.0, 0.0), // left bottom
            vec3d(+1.0, -1.0, 0.0), // right bottom
        };
    }


    TEST(intersection_test, square_contains_center) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(0.0, 0.0, 0.0)))
    }

    TEST(intersection_test, square_contains_corner_top_left) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(-1.0, +1.0, 0.0)))
    }

    TEST(intersection_test, square_contains_corner_top_right) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(+1.0, +1.0, 0.0)))
    }

    TEST(intersection_test, square_contains_corner_bottom_right) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(+1.0, -1.0, 0.0)))
    }

    TEST(intersection_test, square_contains_corner_bottom_left) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(-1.0, -1.0, 0.0)))
    }

    TEST(intersection_test, square_contains_edge_center_left) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(-1.0, 0.0, 0.0)))
    }

    TEST(intersection_test, square_contains_edge_center_top) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(0.0, +1.0, 0.0)))
    }

    TEST(intersection_test, square_contains_edge_center_right) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(+1.0, 0.0, 0.0)))
    }

    TEST(intersection_test, square_contains_edge_center_bottom) {
        CER_ASSERT_TRUE(containsPoint(square(), vec3d(0.0, -1.0, 0.0)))
    }

    TEST(intersection_test, triangle_contains_origin) {
        CER_ASSERT_TRUE(containsPoint(triangle(), vec3d(0.0, 0.0, 0.0)))
    }

    TEST(intersection_test, triangle_contains_corner_top) {
        CER_ASSERT_TRUE(containsPoint(triangle(), vec3d(-1.0, +1.0, 0.0)))
    }

    TEST(intersection_test, triangle_contains_corner_left) {
        CER_ASSERT_TRUE(containsPoint(triangle(), vec3d(-1.0, -1.0, 0.0)))
    }

    TEST(intersection_test, triangle_contains_corner_right) {
        CER_ASSERT_TRUE(containsPoint(triangle(), vec3d(+1.0, -1.0, 0.0)))
    }

    TEST(intersection_test, triangle_contains_edge_center_top_left) {
        CER_ASSERT_TRUE(containsPoint(triangle(), (triangle()[0] + triangle()[1]) / 2.0))
    }

    TEST(intersection_test, triangle_contains_edge_center_top_right) {
        CER_ASSERT_TRUE(containsPoint(triangle(), (triangle()[1] + triangle()[2]) / 2.0))
    }

    TEST(intersection_test, triangle_contains_edge_center_bottom) {
        CER_ASSERT_TRUE(containsPoint(triangle(), (triangle()[2] + triangle()[0]) / 2.0))
    }

    TEST(intersection_test, triangle_contains_outer_point) {
        CER_ASSERT_FALSE(containsPoint(triangle(), vec3d(+1.0, +1.0, 0.0)))
    }

    TEST(intersection_test, intersect_ray_plane) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());
        CER_ASSERT_TRUE(is_nan(intersect_ray_plane(ray, plane3f(vec3f(0.0f, 0.0f, -1.0f), vec3f::pos_z()))))
        CER_ASSERT_FLOAT_EQ(0.0f, intersect_ray_plane(ray, plane3f(vec3f(0.0f, 0.0f,  0.0f), vec3f::pos_z())))
        CER_ASSERT_FLOAT_EQ(1.0f, intersect_ray_plane(ray, plane3f(vec3f(0.0f, 0.0f,  1.0f), vec3f::pos_z())))
    }

    TEST(intersection_test, intersect_ray_triangle) {
        constexpr auto p0 = vec3d(2.0, 5.0, 2.0);
        constexpr auto p1 = vec3d(4.0, 7.0, 2.0);
        constexpr auto p2 = vec3d(3.0, 2.0, 2.0);

        CER_ASSERT_TRUE(is_nan(intersect_ray_triangle(ray3d(vec3d::zero(), vec3d::pos_x()), p0, p1, p2)))
        CER_ASSERT_TRUE(is_nan(intersect_ray_triangle(ray3d(vec3d::zero(), vec3d::pos_y()), p0, p1, p2)))
        CER_ASSERT_TRUE(is_nan(intersect_ray_triangle(ray3d(vec3d::zero(), vec3d::pos_z()), p0, p1, p2)))
        CER_ASSERT_TRUE(is_nan(intersect_ray_triangle(ray3d(vec3d(0.0, 0.0, 2.0), vec3d::pos_y()), p0, p1, p2)))
        CER_ASSERT_DOUBLE_EQ(2.0, intersect_ray_triangle(ray3d(vec3d(3.0, 5.0, 0.0), vec3d::pos_z()), p0, p1, p2))
        CER_ASSERT_DOUBLE_EQ(2.0, intersect_ray_triangle(ray3d(vec3d(2.0, 5.0, 0.0), vec3d::pos_z()), p0, p1, p2))
        CER_ASSERT_DOUBLE_EQ(2.0, intersect_ray_triangle(ray3d(vec3d(4.0, 7.0, 0.0), vec3d::pos_z()), p0, p1, p2))
        CER_ASSERT_DOUBLE_EQ(2.0, intersect_ray_triangle(ray3d(vec3d(3.0, 2.0, 0.0), vec3d::pos_z()), p0, p1, p2))
    }

    TEST(intersection_test, intersect_ray_square) {
        constexpr auto poly = square() + vec3d(0, 0, 1);

        CER_ASSERT_TRUE(is_nan(intersect_ray_polygon(ray3d(vec3d::zero(), vec3d::neg_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly))))
        CER_ASSERT_TRUE(is_nan(intersect_ray_polygon(ray3d(vec3d(2, 2, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly))))
        CER_ASSERT_TRUE(is_nan(intersect_ray_polygon(ray3d(vec3d(-2, 0, 1), vec3d::pos_x()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly))))
        CER_ASSERT_TRUE(is_nan(intersect_ray_polygon(ray3d(vec3d(-2, 0, 0), vec3d::pos_x()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly))))

        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d( 0,  0, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d( 0,  0, 2), vec3d::neg_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d(+1, +1, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d(+1, -1, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d(-1, +1, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d(-1, -1, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d( 0, +1, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d( 0, -1, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d(+1,  0, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
        CER_ASSERT_DOUBLE_EQ(+1.0, intersect_ray_polygon(ray3d(vec3d(-1,  0, 0), vec3d::pos_z()), plane3d(vec3d(0, 0, 1), vec3d::pos_z()), std::begin(poly), std::end(poly)))
    }

    TEST(intersection_test, intersect_ray_bbox) {
        constexpr auto bounds = bbox3f(vec3f(-12.0f, -3.0f,  4.0f), vec3f(  8.0f,  9.0f,  8.0f));

        CER_ASSERT_TRUE(is_nan(intersect_ray_bbox(ray3f(vec3f::zero(), vec3f::neg_z()), bounds)))
        CER_ASSERT_FLOAT_EQ(4.0f, intersect_ray_bbox(ray3f(vec3f::zero(), vec3f::pos_z()), bounds))

        constexpr auto origin = vec3f(-10.0f, -7.0f, 14.0f);
        constexpr auto diff = vec3f(-2.0f, 3.0f, 8.0f) - origin;
        constexpr auto dir = normalize_c(diff);
        ASSERT_FLOAT_EQ(length_c(diff), intersect_ray_bbox(ray3f(origin, dir), bounds));

    }

    TEST(intersection_test, intersect_ray_sphere) {
        const ray3f ray(vec3f::zero(), vec3f::pos_z());

        // ray originates inside sphere and hits at north pole
        ASSERT_FLOAT_EQ(2.0f, intersect_ray_sphere(ray, vec3f::zero(), 2.0f));

        // ray originates outside sphere and hits at south pole
        ASSERT_FLOAT_EQ(3.0f, intersect_ray_sphere(ray, vec3f(0.0f, 0.0f, 5.0f), 2.0f));

        // miss
        ASSERT_TRUE(is_nan(intersect_ray_sphere(ray, vec3f(3.0f, 2.0f, 2.0f), 1.0f)));
    }

    TEST(intersection_test, intersect_ray_torus) {
        ASSERT_FLOAT_EQ(4.0f, intersect_ray_torus(ray3f(vec3f::zero(), vec3f::pos_y()), vec3f::zero(), 5.0f, 1.0f));
        ASSERT_FLOAT_EQ(4.0f, intersect_ray_torus(ray3f(vec3f::zero(), vec3f::pos_x()), vec3f::zero(), 5.0f, 1.0f));

        ASSERT_FLOAT_EQ(4.0f, intersect_ray_torus(ray3f(vec3f(0.0f, -10.0f, 0.0f), vec3f::pos_y()), vec3f::zero(), 5.0f, 1.0f));
        ASSERT_FLOAT_EQ(4.0f, intersect_ray_torus(ray3f(vec3f(-10.0f, 0.0f, 0.0f), vec3f::pos_x()), vec3f::zero(), 5.0f, 1.0f));

        ASSERT_FLOAT_EQ(4.0f, intersect_ray_torus(ray3f(vec3f(0.0f, -5.0f, 5.0f), vec3f::neg_z()), vec3f::zero(), 5.0f, 1.0f));

        ASSERT_FLOAT_EQ(4.0f, intersect_ray_torus(ray3f(vec3f(5.0f, -5.0f, 5.0f), vec3f::neg_z()), vec3f(5.0f, 0.0f, 0.0f), 5.0f, 1.0f));

        ASSERT_TRUE(is_nan(intersect_ray_torus(ray3f(vec3f::zero(), vec3f::pos_z()), vec3f::zero(), 5.0f, 1.0f)));
    }

    TEST(intersection_test, intersect_line_plane) {
        constexpr auto p = plane3f(5.0f, vec3f::pos_z());
        constexpr auto l = line3f(vec3f(0, 0, 15), normalize_c(vec3f(1, 0, -1)));
        CER_ASSERT_EQ(vec3f(10, 0, 5), point_at_distance(l, intersect_line_plane(l, p)))
    }

    TEST(intersection_test, intersect_plane_plane) {
        constexpr auto p1 = plane3f(10.0f, vec3f::pos_z());
        constexpr auto p2 = plane3f(20.0f, vec3f::pos_x());
        const auto line = intersect_plane_plane(p1, p2);;

        ASSERT_TRUE(lineOnPlane(p1, line));
        ASSERT_TRUE(lineOnPlane(p2, line));
    }

    TEST(intersection_test, intersect_plane_plane_parallel) {
        constexpr auto p1 = plane3f(10.0f, vec3f::pos_z());
        constexpr auto p2 = plane3f(11.0f, vec3f::pos_z());
        const line3f line = intersect_plane_plane(p1, p2);;

        ASSERT_EQ(vec3f::zero(), line.direction);
        ASSERT_EQ(vec3f::zero(), line.point);
    }

    TEST(intersection_test, intersect_plane_plane_similar) {
        constexpr auto anchor =  vec3f(100, 100, 100);
        constexpr auto p1 =  plane3f(anchor, vec3f::pos_x());
        const auto p2 =  plane3f(anchor, quatf(vec3f::neg_y(), to_radians(0.5f)) * vec3f::pos_x()); // p1 rotated by 0.5 degrees
        const auto line = intersect_plane_plane(p1, p2);

        ASSERT_TRUE(lineOnPlane(p1, line));
        ASSERT_TRUE(lineOnPlane(p2, line));
    }

    TEST(intersection_test, intersect_plane_plane_too_similar) {
        constexpr auto anchor =  vec3f(100,100,100);
        constexpr auto p1 =  plane3f(anchor, vec3f::pos_x());
        const auto p2 =  plane3f(anchor, quatf(vec3f::neg_y(), to_radians(0.0001f)) * vec3f::pos_x()); // p1 rotated by 0.0001 degrees
        const auto line = intersect_plane_plane(p1, p2);;

        ASSERT_EQ(vec3f::zero(), line.direction);
        ASSERT_EQ(vec3f::zero(), line.point);
    }

    bool lineOnPlane(const plane3f& plane, const line3f& line) {
        if (plane.point_status(line.point) != plane_status::inside){
            return false;
        } else if (plane.point_status(point_at_distance(line, 16.0f)) != plane_status::inside) {
            return false;
        } else {
            return true;
        }
    }
}
