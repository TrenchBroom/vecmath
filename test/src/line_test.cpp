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

#include <vecmath/forward.h>
#include <vecmath/line.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/scalar.h>

namespace vm {
    TEST(line_test, constructor_default) {
        constexpr auto p = line3f();
        CER_ASSERT_EQ(vec3f::zero(), p.point)
        CER_ASSERT_EQ(vec3f::zero(), p.direction)
    }

    TEST(line_test, constructor_convert) {
        constexpr auto l = line3d(vec3d::one(), vec3d::pos_z());
        constexpr auto k = line3f(l);
        CER_ASSERT_VEC_EQ(vec3f::one(), k.point)
        CER_ASSERT_VEC_EQ(vec3f::pos_z(), k.direction)
    }

    TEST(line_test, constructor_with_point_and_direction) {
        constexpr auto p = vec3f(10,20,30);
        constexpr auto n = normalize_c(vec3f(1.0f, 2.0f, 3.0f));
        constexpr auto l = line3f(p, n);
        CER_ASSERT_VEC_EQ(p, l.point)
        CER_ASSERT_VEC_EQ(n, l.direction)
    }

    TEST(line_test, get_origin) {
        constexpr auto l = line3d(vec3d::one(), vec3d::pos_z());
        CER_ASSERT_VEC_EQ(l.point, l.get_origin())
    }

    TEST(line_test, get_direction) {
        constexpr auto l = line3d(vec3d::one(), vec3d::pos_z());
        CER_ASSERT_VEC_EQ(l.direction, l.get_direction());
    }

    TEST(line_test, transform) {
        const auto l = line3d(vec3d::one(), vec3d::pos_z());
        const auto rm = rotation_matrix(to_radians(15.0), to_radians(20.0), to_radians(-12.0));
        const auto tm = translation_matrix(vec3d::one());

        const auto lt = l.transform(rm * tm);
        ASSERT_TRUE(is_unit(l.direction, vm::Cd::almost_zero()));
        ASSERT_VEC_EQ(rm * tm * l.point, lt.point);
        ASSERT_VEC_EQ(rm * l.direction, lt.direction);
    }

    TEST(line_test, transform_c) {
        constexpr auto l = line3d(vec3d::one(), vec3d::pos_z());
        constexpr auto sm = scaling_matrix(vec3d(2.0, 0.5, -2.0));
        constexpr auto tm = translation_matrix(vec3d::one());

        constexpr auto lt = l.transform_c(sm * tm);
        ASSERT_TRUE(is_unit(l.direction, vm::Cd::almost_zero()));
        ASSERT_VEC_EQ(sm * tm * l.point, lt.point);
        ASSERT_VEC_EQ(normalize_c(sm * l.direction), lt.direction);
    }

    TEST(line_test, make_canonical) {
        constexpr auto l1 = line3d(vec3d(-10, 0, 10), vec3d::pos_x());
        constexpr auto l2 = line3d(vec3d(+10, 0, 10), vec3d::pos_x());
        CER_ASSERT_EQ(l1.make_canonical(), l2.make_canonical())
    }

    TEST(line_test, distance_to_projected_point) {
        constexpr auto l = line3f(vec3f(10, 0, 0), vec3f::pos_z());
        CER_ASSERT_FLOAT_EQ(0.0f, distance_to_projected_point(l, vec3f(10, 0, 0)))
        CER_ASSERT_FLOAT_EQ(10.0f, distance_to_projected_point(l, vec3f(10, 0, 10)))
        CER_ASSERT_FLOAT_EQ(10.0f, distance_to_projected_point(l, vec3f(10, 10, 10)))
    }

    TEST(line_test, project_point) {
        constexpr auto l = line3f(vec3f(10,0,0), vec3f::pos_z());
        CER_ASSERT_VEC_EQ(vec3f(10,0,5), project_point(l, vec3f(100, 100, 5)))
    }

    TEST(line_test, is_equal) {
        CER_ASSERT_TRUE(is_equal(line3d(), line3d(), 0.0))
        CER_ASSERT_TRUE(is_equal(line3d(vec3d::zero(), vec3d::pos_z()), line3d(vec3d::zero(), vec3d::pos_z()), 0.0))
        CER_ASSERT_FALSE(is_equal(line3d(vec3d(0, 0, 0), vec3d(0, 0, 1)), line3d(vec3d(1, 0, 0), vec3d(0, 0, 1)), 0.0))
        CER_ASSERT_TRUE(is_equal(line3d(vec3d(0, 0, 0), vec3d(0, 0, 1)), line3d(vec3d(1, 0, 0), vec3d(0, 0, 1)), 2.0))
    }

    TEST(line_test, operator_equal) {
        CER_ASSERT_TRUE(line3d() == line3d())
        CER_ASSERT_TRUE(line3d(vec3d::zero(), vec3d::pos_z()) == line3d(vec3d::zero(), vec3d::pos_z()))
        CER_ASSERT_FALSE(line3d(vec3d(0, 0, 0), vec3d(0, 0, 1)) == line3d(vec3d(1, 0, 0), vec3d(0, 0, 1)))
    }

    TEST(line_test, operator_not_equal) {
        CER_ASSERT_FALSE(line3d() != line3d())
        CER_ASSERT_FALSE(line3d(vec3d::zero(), vec3d::pos_z()) != line3d(vec3d::zero(), vec3d::pos_z()))
        CER_ASSERT_TRUE(line3d(vec3d(0, 0, 0), vec3d(0, 0, 1)) != line3d(vec3d(1, 0, 0), vec3d(0, 0, 1)))
    }
}
