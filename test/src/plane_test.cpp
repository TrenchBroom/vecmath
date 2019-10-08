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

#include <vecmath/forward.h>
#include <vecmath/vec.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/plane.h>
#include <vecmath/scalar.h>
#include <vecmath/constexpr_util.h>

#include "test_utils.h"

#include <array>

namespace vm {
    TEST(plane_test, constructor_default) {
        constexpr auto p = plane3f();
        CER_ASSERT_EQ(0.0f, p.distance)
        CER_ASSERT_EQ(vec3f::zero(), p.normal)
    }

    TEST(plane_test, constructor_convert) {
        constexpr auto p = plane3d(1.0, vec3d::pos_z());
        constexpr auto q = plane3f(p);
        CER_ASSERT_FLOAT_EQ(1.0f, q.distance)
        CER_ASSERT_VEC_EQ(vec3f::pos_z(), q.normal)
    }

    TEST(plane_test, constructor_with_distance_and_normal) {
        constexpr auto  d = 123.0f;
        constexpr auto n = normalize_c(vec3f(1.0f, 2.0f, 3.0f));
        constexpr auto p =  plane3f(d, n);
        CER_ASSERT_FLOAT_EQ(d, p.distance)
        CER_ASSERT_VEC_EQ(n, p.normal)
    }

    TEST(plane_test, constructor_with_anchor_and_normal) {
        constexpr auto a = vec3f(-2038.034f, 0.0023f, 32.0f);
        constexpr auto n = normalize_c(vec3f(9.734f, -3.393f, 2.033f));
        constexpr auto p = plane3f(a, n);
        CER_ASSERT_FLOAT_EQ(dot(a, n), p.distance)
        CER_ASSERT_VEC_EQ(n, p.normal)
    }

    TEST(plane_test, anchor) {
        constexpr auto a = vec3f(-2038.034f, 0.0023f, 32.0f);
        constexpr auto n = normalize_c(vec3f(9.734f, -3.393f, 2.033f));
        constexpr auto p =  plane3f(a, n);
        CER_ASSERT_VEC_EQ(p.distance * n, p.anchor())
    }

    TEST(plane_test, at) {
        constexpr auto a = vec3f(-2038.034f, 0.0023f, 32.0f);
        constexpr auto n = normalize_c(vec3f(9.734f, -3.393f, 2.033f));
        constexpr auto p = plane3f(a, n);
        constexpr auto point1 = vec2f(27.022f, -12.0123223f);

        CER_ASSERT_FLOAT_EQ((p.distance - point1.x() * p.normal.y() - point1.y() * p.normal.z()) / p.normal[axis::x],
                        p.at(point1, axis::x))
        CER_ASSERT_FLOAT_EQ((p.distance - point1.x() * p.normal.x() - point1.y() * p.normal.z()) / p.normal[axis::y],
                        p.at(point1, axis::y))
        CER_ASSERT_FLOAT_EQ((p.distance - point1.x() * p.normal.x() - point1.y() * p.normal.y()) / p.normal[axis::z],
                        p.at(point1, axis::z))
    }

    TEST(plane_test, at_parallel_planes) {
        constexpr auto p1 = plane3f(10.0f, vec3f::pos_x());

        CER_ASSERT_FLOAT_EQ(p1.distance, p1.at(vec2f(2.0f, 1.0f), axis::x))
        CER_ASSERT_FLOAT_EQ(p1.distance, p1.at(vec2f(22.0f, -34322.0232f), axis::x))
        CER_ASSERT_FLOAT_EQ(0.0f, p1.at(vec2f(2.0f, 1.0f), axis::y))
        CER_ASSERT_FLOAT_EQ(0.0f, p1.at(vec2f(22.0f, -34322.0232f), axis::y))
        CER_ASSERT_FLOAT_EQ(0.0f, p1.at(vec2f(2.0f, 1.0f), axis::z))
        CER_ASSERT_FLOAT_EQ(0.0f, p1.at(vec2f(22.0f, -34322.0232f), axis::z))
    }

    TEST(plane_test, xyz_at) {
        constexpr auto a = vec3f(-2038.034f, 0.0023f, 32.0f);
        constexpr auto n = normalize_c(vec3f(9.734f, -3.393f, 2.033f));
        constexpr auto p = plane3f(a, n);
        constexpr auto point1 = vec2f(27.022f, -12.0123223f);

        CER_ASSERT_FLOAT_EQ(p.at(point1, axis::x), p.xAt(point1))
        CER_ASSERT_FLOAT_EQ(p.at(point1, axis::y), p.yAt(point1))
        CER_ASSERT_FLOAT_EQ(p.at(point1, axis::z), p.zAt(point1))
    }

    TEST(plane_test, point_distance) {
        constexpr auto a = vec3f(-2038.034f, 0.0023f, 32.0f);
        constexpr auto n = normalize_c(vec3f(9.734f, -3.393f, 2.033f));
        constexpr auto p = plane3f(a, n);
        constexpr auto point = vec3f(1.0f, -32.37873f, 32.0f);
        CER_ASSERT_EQ(dot(point, p.normal) - p.distance, p.point_distance(point))
    }

    TEST(plane_test, point_status) {
        constexpr auto p = plane3f(10.0f, vec3f::pos_z());
        CER_ASSERT_EQ(plane_status::above, p.point_status(vec3f(0.0f, 0.0f, 11.0f)))
        CER_ASSERT_EQ(plane_status::below, p.point_status(vec3f(0.0f, 0.0f, 9.0f)))
        CER_ASSERT_EQ(plane_status::inside, p.point_status(vec3f(0.0f, 0.0f, 10.0f)))
    }

    TEST(plane_test, flip) {
        constexpr auto p = plane3f(10.0f, vec3f::pos_z());
        CER_ASSERT_EQ(plane3f(-10.0f, vec3f::neg_z()), p.flip());
    }

    TEST(plane_test, transform) {
        const auto p = plane3d(vec3d::one(), vec3d::pos_z());
        const auto rm = rotation_matrix(to_radians(15.0), to_radians(20.0), to_radians(-12.0));
        const auto tm = translation_matrix(vec3d::one());

        const auto pt = p.transform(rm * tm);
        ASSERT_TRUE(is_unit(p.normal, vm::Cd::almostZero()));
        ASSERT_EQ(plane_status::inside, pt.point_status(rm * tm * p.anchor()));
        ASSERT_VEC_EQ(rm * p.normal, pt.normal);
    }

    TEST(plane_test, transform_c) {
        constexpr auto p = plane3d(vec3d::one(), vec3d::pos_z());
        constexpr auto sm = scaling_matrix(vec3d(2.0, 0.5, 3.0));
        constexpr auto tm = translation_matrix(vec3d::one());

        constexpr auto pt = p.transform_c(sm * tm);
        CER_ASSERT_TRUE(is_unit_c(p.normal, vm::Cd::almostZero()))
        CER_ASSERT_EQ(plane_status::inside, pt.point_status(sm * tm * p.anchor()))
        CER_ASSERT_VEC_EQ(normalize_c(sm * p.normal), pt.normal)
    }

    TEST(plane_test, project_point) {
        CER_ASSERT_VEC_EQ(vec3d(0, 0, 0), plane3d(0.0, vec3d::pos_z()).project_point(vec3d(0, 0, 10)))
        CER_ASSERT_VEC_EQ(vec3d(1, 2, 0), plane3d(0.0, vec3d::pos_z()).project_point(vec3d(1, 2, 10)))
        CER_ASSERT_VEC_EQ(vec3d(0, 0, 0), plane3d(0.0, normalize_c(vec3d(1, 1, 1))).project_point(vec3d(10, 10, 10)))
    }

    TEST(plane_test, project_point_direction) {
        CER_ASSERT_VEC_EQ(vec3d(0, 0, 0), plane3d(0.0, vec3d::pos_z()).project_point(vec3d(0, 0, 10), vec3d::pos_z()))
        CER_ASSERT_VEC_EQ(vec3d(1, 2, 0), plane3d(0.0, vec3d::pos_z()).project_point(vec3d(1, 2, 10), vec3d::pos_z()))
        CER_ASSERT_VEC_EQ(vec3d(0, 0, 0),
            plane3d(0.0, vec3d::pos_z()).project_point(vec3d(10, 10, 10), normalize_c(vec3d(1, 1, 1))))
    }

    TEST(plane_test, project_vector) {
        CER_ASSERT_VEC_EQ(vec3d(1, 1, 0), plane3d(0.0, vec3d::pos_z()).project_vector(vec3d(1, 1, 1)))
        CER_ASSERT_VEC_EQ(vec3d(1, 1, 0), plane3d(1.0, vec3d::pos_z()).project_vector(vec3d(1, 1, 1)))
    }

    TEST(plane_test, project_vector_direction) {
        CER_ASSERT_VEC_EQ(vec3d(1, 1, 0), plane3d(0.0, vec3d::pos_z()).project_vector(vec3d(1, 1, 1), vec3d::pos_z()))
        CER_ASSERT_VEC_EQ(vec3d(1, 1, 0), plane3d(1.0, vec3d::pos_z()).project_vector(vec3d(1, 1, 1), vec3d::pos_z()))
        CER_ASSERT_VEC_EQ(vec3d(2, 2, 0),
            plane3d(0.0, vec3d::pos_z()).project_vector(vec3d(1, 1, 1), normalize_c(vec3d(1, 1, -1))))
    }

    TEST(plane_test, is_equal) {
        CER_ASSERT_TRUE(is_equal(plane3f(0.0f, vec3f::pos_x()), plane3f(0.0f, vec3f::pos_x()), constants<float>::almostZero()))
        CER_ASSERT_TRUE(is_equal(plane3f(0.0f, vec3f::pos_y()), plane3f(0.0f, vec3f::pos_y()), constants<float>::almostZero()))
        CER_ASSERT_TRUE(is_equal(plane3f(0.0f, vec3f::pos_z()), plane3f(0.0f, vec3f::pos_z()), constants<float>::almostZero()))
        CER_ASSERT_FALSE(is_equal(plane3f(0.0f, vec3f::pos_x()), plane3f(0.0f, vec3f::neg_x()), constants<float>::almostZero()))
        CER_ASSERT_FALSE(is_equal(plane3f(0.0f, vec3f::pos_x()), plane3f(0.0f, vec3f::pos_y()), constants<float>::almostZero()))
    }

    TEST(plane_test, operator_equal) {
        CER_ASSERT_TRUE (plane3d() == plane3d())
        CER_ASSERT_TRUE (plane3d(10.0, vec3d::pos_z()) == plane3d(10.0, vec3d::pos_z()))
        CER_ASSERT_FALSE(plane3d(20.0, vec3d::pos_z()) == plane3d(10.0, vec3d::pos_z()))
        CER_ASSERT_FALSE(plane3d(10.0, vec3d::neg_z()) == plane3d(10.0, vec3d::pos_z()))
        CER_ASSERT_FALSE(plane3d(10.0, normalize_c(vec3d::one())) == plane3d(10.0, vec3d::pos_z()))
    }

    TEST(plane_test, operator_not_equal) {
        CER_ASSERT_FALSE(plane3d() != plane3d())
        CER_ASSERT_FALSE(plane3d(10.0, vec3d::pos_z()) != plane3d(10.0, vec3d::pos_z()))
        CER_ASSERT_TRUE (plane3d(20.0, vec3d::pos_z()) != plane3d(10.0, vec3d::pos_z()))
        CER_ASSERT_TRUE (plane3d(10.0, vec3d::neg_z()) != plane3d(10.0, vec3d::pos_z()))
        CER_ASSERT_TRUE (plane3d(10.0, normalize_c(vec3d::one())) != plane3d(10.0, vec3d::pos_z()))
    }

    template <typename T>
    void assertValidPlaneNormal(const vec<T,3>& expected, const vec<T,3>& p1, const vec<T,3>& p2, const vec<T,3>& p3) {
        const auto result = plane_normal(p1, p2, p3);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_VEC_EQ(expected, std::get<1>(result));
    }

    template <typename T>
    void assertInvalidPlaneNormal(const vec<T,3>& p1, const vec<T,3>& p2, const vec<T,3>& p3) {
        const auto result = plane_normal(p1, p2, p3);
        ASSERT_FALSE(std::get<0>(result));
    }

    TEST(plane_test, plane_normal) {
        assertValidPlaneNormal(vec3d::pos_z(), vec3d::zero(), vec3d::pos_y(), vec3d::pos_x());
        assertValidPlaneNormal(vec3d::pos_z(), vec3d::zero(), normalize(vec3d(1, 1, 0)), vec3d::pos_x());
        assertInvalidPlaneNormal(vec3d::zero(), vec3d::zero(), vec3d::pos_x());
        assertInvalidPlaneNormal(vec3d::zero(), vec3d::pos_x(), vec3d::pos_x());
        assertInvalidPlaneNormal(vec3d::zero(), vec3d::neg_x(), vec3d::pos_x());
        assertInvalidPlaneNormal(vec3d::zero(), vec3d::zero(), vec3d::pos_x());
    }

    TEST(plane_test, from_points) {
        bool valid;
        plane3f plane;
        std::array<vec3f, 3> points;
        const float epsilon = constants<float>::pointStatusEpsilon();

        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(0.0f, 1.0f, 0.0f);
        points[2] = vec3f(1.0f, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_TRUE(valid);
        ASSERT_VEC_EQ(vec3f::pos_z(), plane.normal);
        ASSERT_FLOAT_EQ(0.0f, plane.distance);

        // right angle, short vectors
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(0.0f, epsilon, 0.0f);
        points[2] = vec3f(epsilon, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_TRUE(valid);
        ASSERT_VEC_EQ(vec3f::pos_z(), plane.normal);
        ASSERT_FLOAT_EQ(0.0f, plane.distance);

        // plane point vectors at a 45 degree angle, short vectors
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(epsilon, epsilon, 0.0f);
        points[2] = vec3f(epsilon, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_TRUE(valid);
        ASSERT_VEC_EQ(vec3f::pos_z(), plane.normal);
        ASSERT_FLOAT_EQ(0.0f, plane.distance);

        // horizontal plane at z=length units above the origin
        points[0] = vec3f(0.0f, 0.0f, epsilon);
        points[1] = vec3f(0.0f, epsilon, epsilon);
        points[2] = vec3f(epsilon, 0.0f, epsilon);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_TRUE(valid);
        ASSERT_VEC_EQ(vec3f::pos_z(), plane.normal);
        ASSERT_FLOAT_EQ(epsilon, plane.distance);

        // small angle (triangle 1000 units wide, length units tall)
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(1000.0f, epsilon, 0.0f);
        points[2] = vec3f(1000.0f, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_TRUE(valid);
        ASSERT_VEC_EQ(vec3f::pos_z(), plane.normal);
        ASSERT_FLOAT_EQ(0.0f, plane.distance);

        // small angle
        points[0] = vec3f(224.0f, -400.0f, 1648.0f);
        points[1] = vec3f(304.0f, -432.0f, 1248.0f + epsilon);
        points[2] = vec3f(304.0f, -432.0f, 1248.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_TRUE(valid);
        ASSERT_FLOAT_EQ(1.0f, length(plane.normal));

        // too-small angle (triangle 1000 units wide, length/100 units tall)
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(1000.0f, epsilon/100.0f, 0.0f);
        points[2] = vec3f(1000.0f, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_FALSE(valid);

        // all zero
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(0.0f, 0.0f, 0.0f);
        points[2] = vec3f(0.0f, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_FALSE(valid);

        // same direction, short vectors
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(2*epsilon, 0.0f, 0.0f);
        points[2] = vec3f(epsilon, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_FALSE(valid);

        // opposite, short vectors
        points[0] = vec3f(0.0f, 0.0f, 0.0f);
        points[1] = vec3f(-epsilon, 0.0f, 0.0f);
        points[2] = vec3f(epsilon, 0.0f, 0.0f);

        std::tie(valid, plane) = from_points(std::begin(points), std::end(points));
        ASSERT_FALSE(valid);
    }

    TEST(plane_test, horizontal_plane) {
        constexpr auto position = vec3f(322.0f, -122.2392f, 34.0f);
        constexpr auto p = horizontal_plane(position);
        CER_ASSERT_TRUE(p.point_status(position) == plane_status::inside)
        CER_ASSERT_VEC_EQ(vec3f::pos_z(), p.normal)
    }

    TEST(plane_test, orthogonal_plane) {
        const auto position = vec3f(322.0f, -122.2392f, 34.0f);
        const auto direction = normalize(vec3f(1.0f, 2.0f, -3.0f));
        const auto p = orthogonal_plane(position, direction);
        ASSERT_TRUE(p.point_status(position) == plane_status::inside);
        ASSERT_VEC_EQ(direction, p.normal);
    }

    TEST(plane_test, aligned_orthogonal_plane) {
        constexpr auto position = vec3f(322.0f, -122.2392f, 34.0f);
        constexpr auto direction = normalize_c(vec3f(1.0f, 2.0f, -3.0f));
        constexpr auto p = aligned_orthogonal_plane(position, direction);
        CER_ASSERT_TRUE(p.point_status(position) == plane_status::inside)
        CER_ASSERT_VEC_EQ(vec3f::neg_z(), p.normal)
    }
}
