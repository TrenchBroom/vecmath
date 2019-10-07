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
#include <vecmath/vec_ext.h>

#include "test_utils.h"

#include <array>
#include <limits>
#include <string>
#include <vector>

namespace vm {
    TEST(vec_test, default_constructor) {
        constexpr auto v = vec3f();
        ASSERT_EQ(0.0f, v[0]);
        ASSERT_EQ(0.0f, v[1]);
        ASSERT_EQ(0.0f, v[2]);
    }

    TEST(vec_test, initializer_list_constructor) {
        constexpr auto v = vec3f({1.0f, 2.0f, 3.0f});
        ASSERT_EQ(vec3f(1, 2, 3), v);
    }

    TEST(vec_test, componentwise_constructor_with_matching_type) {
        constexpr auto v = vec3f(1.0f, 2.0f, 3.0f);
        ASSERT_EQ(1.0f, v[0]);
        ASSERT_EQ(2.0f, v[1]);
        ASSERT_EQ(3.0f, v[2]);
    }

    TEST(vec_test, componentwise_constructor_with_mixed_types) {
        constexpr auto v = vec3f(1.0, 2.0f, 3u);
        ASSERT_EQ(1.0f, v[0]);
        ASSERT_EQ(2.0f, v[1]);
        ASSERT_EQ(3.0f, v[2]);
    }

    TEST(vec_test, converting_constructor) {
        constexpr auto vf = vec3f(1.0f, 2.0f, 3.0f);
        constexpr auto vd = vec3d(vf);
        ASSERT_EQ(static_cast<double>(vf[0]), vd[0]);
        ASSERT_EQ(static_cast<double>(vf[1]), vd[1]);
        ASSERT_EQ(static_cast<double>(vf[2]), vd[2]);
    }

    TEST(vec_test, embedding_constructor) {
        constexpr auto vf = vec2f(1.0f, 2.0f);
        constexpr auto vd = vec3d(vf, 3.0f);
        ASSERT_EQ(static_cast<double>(vf[0]), vd[0]);
        ASSERT_EQ(static_cast<double>(vf[1]), vd[1]);
        ASSERT_EQ(static_cast<double>(3.0f),  vd[2]);
    }

    TEST(vec_test, assignment) {
        constexpr auto t = vec3f(2.0f, 3.0f, 5.0f);
        constexpr auto v = t;
        ASSERT_EQ(t, v);
    }

    TEST(vec_test, fill) {
        constexpr auto v1 = vec3f::fill( 2.0f);
        constexpr auto v2 = vec3f::fill( 0.0f);
        constexpr auto v3 = vec3f::fill(-2.0f);
        ASSERT_EQ(vec3f( 2.0f,  2.0f,  2.0f), v1);
        ASSERT_EQ(vec3f( 0.0f,  0.0f,  0.0f), v2);
        ASSERT_EQ(vec3f(-2.0f, -2.0f, -2.0f), v3);
    }

    TEST(vec_test, axis) {
        constexpr auto vx = vec3f::axis(0);
        constexpr auto vy = vec3f::axis(1);
        constexpr auto vz = vec3f::axis(2);
        ASSERT_EQ(vec3f(1, 0, 0), vx);
        ASSERT_EQ(vec3f(0, 1, 0), vy);
        ASSERT_EQ(vec3f(0, 0, 1), vz);
    }

    TEST(vec_test, operator_subscript) {
        // aggregate initialization circumvents constructors and constructs the value array in place
        constexpr auto v = vec4f{ { 1.0f, 2.0f, 3.0f, 4.0f } };
        ASSERT_EQ(1.0f, v[0]);
        ASSERT_EQ(2.0f, v[1]);
        ASSERT_EQ(3.0f, v[2]);
        ASSERT_EQ(4.0f, v[3]);
    }

    TEST(vec_test, accessors) {
        // aggregate initialization circumvents constructors and constructs the value array in place
        constexpr auto v = vec4f{ { 1.0f, 2.0f, 3.0f, 4.0f } };
        constexpr auto vx = v.x();
        constexpr auto vy = v.y();
        constexpr auto vz = v.z();
        constexpr auto vw = v.w();
        constexpr auto vxy = v.xy();
        constexpr auto vxyz = v.xyz();
        constexpr auto vxyzw = v.xyzw();

        ASSERT_EQ(v[0], vx);
        ASSERT_EQ(v[1], vy);
        ASSERT_EQ(v[2], vz);
        ASSERT_EQ(v[3], vw);
        ASSERT_EQ(vec2f(1.0f, 2.0f), vxy);
        ASSERT_EQ(vec3f(1.0f, 2.0f, 3.0f), vxyz);
        ASSERT_EQ(v, vxyzw);
    }

    TEST(vec_test, static_members) {
        constexpr auto pos_x = vec3f::pos_x();
        constexpr auto pos_y = vec3f::pos_y();
        constexpr auto pos_z = vec3f::pos_z();
        constexpr auto neg_x = vec3f::neg_x();
        constexpr auto neg_y = vec3f::neg_y();
        constexpr auto neg_z = vec3f::neg_z();
        constexpr auto zero  = vec3f::zero();
        constexpr auto one   = vec3f::one();
        constexpr auto nan   = vec3f::nan();
        constexpr auto min   = vec3f::min();
        constexpr auto max   = vec3f::max();

        ASSERT_EQ(vec3f(+1,  0,  0), pos_x);
        ASSERT_EQ(vec3f( 0, +1,  0), pos_y);
        ASSERT_EQ(vec3f( 0,  0, +1), pos_z);
        ASSERT_EQ(vec3f(-1,  0,  0), neg_x);
        ASSERT_EQ(vec3f( 0, -1,  0), neg_y);
        ASSERT_EQ(vec3f( 0,  0, -1), neg_z);
        ASSERT_EQ(vec3f( 0,  0,  0), zero);
        ASSERT_EQ(vec3f( 1,  1,  1), one);

        for (size_t i = 0; i < 3; ++i) {
            ASSERT_FLOAT_EQ(std::numeric_limits<float>::min(), min[i]);
            ASSERT_FLOAT_EQ(std::numeric_limits<float>::max(), max[i]);
            ASSERT_TRUE(is_nan(nan[i]));
        }
    }

    // ========== comparison operators ==========

    TEST(vec_test, compare) {
        CER_ASSERT_EQ( 0, compare(vec3f::zero(), vec3f::zero()))
        CER_ASSERT_EQ(-1, compare(vec3f::zero(), vec3f::one()))
        CER_ASSERT_EQ(-1, compare(vec3f::one(),  vec3f(2, 1, 1)))
        CER_ASSERT_EQ(-1, compare(vec3f::one(),  vec3f(1, 2, 1)))
        CER_ASSERT_EQ(-1, compare(vec3f::one(),  vec3f(1, 1, 2)))
        CER_ASSERT_EQ(-1, compare(vec3f::one(),  vec3f(2, 0, 0)))
        CER_ASSERT_EQ(-1, compare(vec3f::one(),  vec3f(1, 2, 0)))

        CER_ASSERT_EQ(+1, compare(vec3f::one(),     vec3f::zero()))
        CER_ASSERT_EQ(+1, compare(vec3f(2, 1, 1), vec3f::one()))
        CER_ASSERT_EQ(+1, compare(vec3f(1, 2, 1), vec3f::one()))
        CER_ASSERT_EQ(+1, compare(vec3f(1, 1, 2), vec3f::one()))
        CER_ASSERT_EQ(+1, compare(vec3f(2, 0, 0), vec3f::one()))
        CER_ASSERT_EQ(+1, compare(vec3f(1, 2, 0), vec3f::one()))

        CER_ASSERT_NE( 0, compare(vec3f(1, 2, 0), vec3f::nan()))
        CER_ASSERT_NE( 0, compare(vec3f::nan(), vec3f(1, 2, 0)))
        // This is inconsistent with how operator== on two float values that are nan returns false,
        // but it is consistent with the totalOrder() function from IEEE 754-2008
        // It's unclear what we should do here and this may need revisiting.
        ASSERT_EQ( 0, compare(vec3f::nan(), vec3f::nan()));
    }

    TEST(vec_test, compare_ranges) {
        constexpr auto r1 = std::array { vec3f(1, 2, 3), vec3f(1, 2, 3) };
        constexpr auto r2 = std::array { vec3f(1, 2, 3), vec3f(2, 2, 3) };
        constexpr auto r3 = std::array { vec3f(2, 2, 3) };

        // same length
        CER_ASSERT_EQ( 0, compare<float>(std::begin(r1), std::end(r1), std::begin(r1), std::end(r1)))
        CER_ASSERT_EQ(-1, compare<float>(std::begin(r1), std::end(r1), std::begin(r2), std::end(r2)))
        CER_ASSERT_EQ(+1, compare<float>(std::begin(r2), std::end(r2), std::begin(r1), std::end(r1)))

        // prefi
        CER_ASSERT_EQ(-1, compare<float>(std::begin(r1), std::next(std::begin(r1)), std::begin(r1), std::end(r1)))
        CER_ASSERT_EQ(+1, compare<float>(std::begin(r1), std::end(r1), std::begin(r1), std::next(std::begin(r1))))

        // different length and not prefi
        CER_ASSERT_EQ(-1, compare<float>(std::begin(r1), std::end(r1), std::begin(r3), std::end(r3)))
        CER_ASSERT_EQ(+1, compare<float>(std::begin(r3), std::end(r3), std::begin(r1), std::end(r1)))
    }

    TEST(vec_test, is_equal) {
        CER_ASSERT_TRUE(is_equal(vec2f::zero(), vec2f::zero(), 0.0f))
        CER_ASSERT_FALSE(is_equal(vec2f::zero(), vec2f::one(), 0.0f))
        CER_ASSERT_TRUE(is_equal(vec2f::zero(), vec2f::one(), 2.0f))

        // nan
        CER_ASSERT_FALSE(is_equal(vec2f::zero(), vec2f::nan(), 0.0f))
        CER_ASSERT_FALSE(is_equal(vec2f::nan(),  vec2f::zero(), 0.0f))
        CER_ASSERT_FALSE(is_equal(vec2f::zero(), vec2f::nan(), 2.0f))
        CER_ASSERT_FALSE(is_equal(vec2f::nan(), vec2f::zero(), 2.0f))

        // See comment in vec_test::compare.
        ASSERT_TRUE(is_equal(vec2f::nan(), vec2f::nan(), 0.0f));
        ASSERT_TRUE(is_equal(vec2f::nan(), vec2f::nan(), 2.0f));
    }

    TEST(vec_test, operator_equal) {
        CER_ASSERT_FALSE(vec3f(1, 2, 3) == vec3f(2, 2, 2))
        CER_ASSERT_TRUE (vec3f(1, 2, 3) == vec3f(1, 2, 3))
        CER_ASSERT_FALSE(vec3f(1, 2, 4) == vec3f(1, 2, 2))

        // NaN
        CER_ASSERT_FALSE(vec2f::zero() == vec2f::nan())
        CER_ASSERT_FALSE(vec2f::nan() == vec2f::zero())

        // See comment in vec_test::compare.
        ASSERT_TRUE(vec2f::nan() == vec2f::nan());
    }

    TEST(vec_test, operator_not_equal) {
        CER_ASSERT_TRUE (vec3f(1, 2, 3) != vec3f(2, 2, 2))
        CER_ASSERT_FALSE(vec3f(1, 2, 3) != vec3f(1, 2, 3))
        CER_ASSERT_TRUE (vec3f(1, 2, 4) != vec3f(1, 2, 2))

        // NaN
        CER_ASSERT_TRUE(vec2f::zero() != vec2f::nan())
        CER_ASSERT_TRUE(vec2f::nan() != vec2f::zero())

        // See comment in vec_test::compare.
        ASSERT_FALSE(vec2f::nan() != vec2f::nan());
    }

    TEST(vec_test, operator_less_than) {
        CER_ASSERT_TRUE (vec3f(1, 2, 3) < vec3f(2, 2, 2))
        CER_ASSERT_FALSE(vec3f(1, 2, 3) < vec3f(1, 2, 3))
        CER_ASSERT_FALSE(vec3f(1, 2, 4) < vec3f(1, 2, 2))
    }

    TEST(vec_test, operator_less_than_or_equal) {
        CER_ASSERT_TRUE (vec3f(1, 2, 3) <= vec3f(2, 2, 2))
        CER_ASSERT_TRUE (vec3f(1, 2, 3) <= vec3f(1, 2, 3))
        CER_ASSERT_FALSE(vec3f(1, 2, 4) <= vec3f(1, 2, 2))
    }

    TEST(vec_test, operator_greater_than) {
        CER_ASSERT_FALSE(vec3f(1, 2, 3) > vec3f(2, 2, 2))
        CER_ASSERT_FALSE(vec3f(1, 2, 3) > vec3f(1, 2, 3))
        CER_ASSERT_TRUE (vec3f(1, 2, 4) > vec3f(1, 2, 2))
    }

    TEST(vec_test, operator_greater_than_or_equal) {
        CER_ASSERT_FALSE(vec3f(1, 2, 3) >= vec3f(2, 2, 2))
        CER_ASSERT_TRUE (vec3f(1, 2, 3) >= vec3f(1, 2, 3))
        CER_ASSERT_TRUE (vec3f(1, 2, 4) >= vec3f(1, 2, 2))
    }

    /* ========== finding components ========== */

    TEST(vec_test, find_max_component) {
        CER_ASSERT_EQ(0u, find_max_component(vec3f::pos_x(), 0))
        CER_ASSERT_NE(0u, find_max_component(vec3f::neg_x(), 0))
        CER_ASSERT_EQ(1u, find_max_component(vec3f::pos_y(), 0))
        CER_ASSERT_NE(1u, find_max_component(vec3f::neg_y(), 0))
        CER_ASSERT_EQ(2u, find_max_component(vec3f::pos_z(), 0))
        CER_ASSERT_NE(2u, find_max_component(vec3f::neg_z(), 0))

        CER_ASSERT_EQ(0u, find_max_component(vec3f(3.0f, 1.0f, -2.0f), 0))
        CER_ASSERT_EQ(1u, find_max_component(vec3f(3.0f, 1.0f, -2.0f), 1))
        CER_ASSERT_EQ(2u, find_max_component(vec3f(3.0f, 1.0f, -2.0f), 2))
        CER_ASSERT_EQ(1u, find_max_component(normalize_c(vec3f(1.0f, 2.0f, -3.0f)), 0))
    }

    TEST(vec_test, find_abs_max_component) {
        CER_ASSERT_EQ(0u, find_abs_max_component(vec3f::pos_x(), 0))
        CER_ASSERT_EQ(0u, find_abs_max_component(vec3f::neg_x(), 0))
        CER_ASSERT_EQ(1u, find_abs_max_component(vec3f::pos_y(), 0))
        CER_ASSERT_EQ(1u, find_abs_max_component(vec3f::neg_y(), 0))
        CER_ASSERT_EQ(2u, find_abs_max_component(vec3f::pos_z(), 0))
        CER_ASSERT_EQ(2u, find_abs_max_component(vec3f::neg_z(), 0))

        CER_ASSERT_EQ(0u, find_abs_max_component(vec3f(3.0f, 1.0f, -2.0f), 0))
        CER_ASSERT_EQ(2u, find_abs_max_component(vec3f(3.0f, 1.0f, -2.0f), 1))
        CER_ASSERT_EQ(1u, find_abs_max_component(vec3f(3.0f, 1.0f, -2.0f), 2))
        CER_ASSERT_EQ(2u, find_abs_max_component(normalize_c(vec3f(1.0f, 2.0f, -3.0f)), 0))
    }

    TEST(vec_test, get_max_component) {
        CER_ASSERT_EQ(1.0f, get_max_component(vec3f::pos_x(), 0))
        CER_ASSERT_EQ(0.0f, get_max_component(vec3f::neg_x(), 0))
        CER_ASSERT_EQ(1.0f, get_max_component(vec3f::pos_y(), 0))
        CER_ASSERT_EQ(0.0f, get_max_component(vec3f::neg_y(), 0))
        CER_ASSERT_EQ(1.0f, get_max_component(vec3f::pos_z(), 0))
        CER_ASSERT_EQ(0.0f, get_max_component(vec3f::neg_z(), 0))

        CER_ASSERT_EQ( 3.0f, get_max_component(vec3f(3.0f, 1.0f, -2.0f), 0))
        CER_ASSERT_EQ( 1.0f, get_max_component(vec3f(3.0f, 1.0f, -2.0f), 1))
        CER_ASSERT_EQ(-2.0f, get_max_component(vec3f(3.0f, 1.0f, -2.0f), 2))
    }

    TEST(vec_test, get_abs_max_component) {
        CER_ASSERT_EQ( 1.0f, get_abs_max_component(vec3f::pos_x(), 0))
        CER_ASSERT_EQ(-1.0f, get_abs_max_component(vec3f::neg_x(), 0))
        CER_ASSERT_EQ( 1.0f, get_abs_max_component(vec3f::pos_y(), 0))
        CER_ASSERT_EQ(-1.0f, get_abs_max_component(vec3f::neg_y(), 0))
        CER_ASSERT_EQ( 1.0f, get_abs_max_component(vec3f::pos_z(), 0))
        CER_ASSERT_EQ(-1.0f, get_abs_max_component(vec3f::neg_z(), 0))

        CER_ASSERT_EQ( 3.0f, get_abs_max_component(vec3f(3.0f, 1.0f, -2.0f), 0))
        CER_ASSERT_EQ(-2.0f, get_abs_max_component(vec3f(3.0f, 1.0f, -2.0f), 1))
        CER_ASSERT_EQ( 1.0f, get_abs_max_component(vec3f(3.0f, 1.0f, -2.0f), 2))
    }

    // ========== arithmetic operators ==========

    TEST(vec_test, operator_unary_plus) {
        CER_ASSERT_EQ(vec3f(+1.0f, -2.0f, +3.0f), +vec3f(+1.0f, -2.0f, +3.0f))
    }

    TEST(vec_test, operator_unary_minus) {
        CER_ASSERT_EQ(vec3f(-1.0f, +2.0f, -3.0f), -vec3f(+1.0f, -2.0f, +3.0f))
    }

    TEST(vec_test, operator_binary_plus) {
        CER_ASSERT_EQ(vec3f(4.0f, 4.0f, 4.0f), vec3f(1.0f, 2.0f, 3.0f) + vec3f(3.0f, 2.0f, 1.0f))
    }

    TEST(vec_test, operator_binary_minus) {
        CER_ASSERT_EQ(vec3f(1.0f, 1.0f, -1.0f), vec3f(2.0f, 3.0f,  1.0f) - vec3f(1.0f, 2.0f,  2.0f))
    }

    TEST(vec_test, operator_multiply_vectors) {
        CER_ASSERT_EQ(vec3f(2.0f, 6.0f, -2.0f), vec3f(2.0f, 3.0f, -1.0f) * vec3f(1.0f, 2.0f,  2.0f))
    }

    TEST(vec_test, operator_multiply_scalar) {
        CER_ASSERT_EQ(vec3f(6.0f, 9.0f, 3.0f), vec3f(2.0f, 3.0f, 1.0f) * 3.0f);
        CER_ASSERT_EQ(vec3f(6.0f, 9.0f, 3.0f), 3.0f * vec3f(2.0f, 3.0f, 1.0f));
    }

    TEST(vec_test, operator_divide_vectors) {
        CER_ASSERT_EQ(vec3f(2.0f,  6.0f, -2.0f), vec3f(2.0f, 12.0f,  2.0f) / vec3f(1.0f,  2.0f, -1.0f))
    }

    TEST(vec_test, operator_divide_scalar) {
        CER_ASSERT_EQ(vec3f(1.0f, 18.0f, 2.0f), vec3f(2.0f, 36.0f, 4.0f) / 2.0f)
        CER_ASSERT_EQ(vec3f(4.0f, 1.0f, -2.0f), 8.0f / vec3f(2.0f, 8.0f, -4.0f))
    }

    // ========== arithmetic functions ==========

    TEST(vec_test, min) {
        CER_ASSERT_EQ(vec3f(+2, +2, +2), min(vec3f(+2, +2, +2), vec3f(+3, +3, +3)))
        CER_ASSERT_EQ(vec3f(-2, -2, -2), min(vec3f(-2, -2, -2), vec3f(-1, -1, -1)))
        CER_ASSERT_EQ(vec3f(+1, +2, +1), min(vec3f(+2, +2, +2), vec3f(+1, +3, +1)))
        CER_ASSERT_EQ(vec3f(-2, -3, -2), min(vec3f(-2, -2, -2), vec3f(-1, -3, -1)))
        CER_ASSERT_EQ(vec3f(-2, -3, -4), min(vec3f(-2, -2, -2), vec3f(-1, -3, -1), vec3f(-1, -3, -4)))
    }

    TEST(vec_test, max) {
        CER_ASSERT_EQ(vec3f(+3, +3, +3), max(vec3f(+2, +2, +2), vec3f(+3, +3, +3)))
        CER_ASSERT_EQ(vec3f(-1, -1, -1), max(vec3f(-2, -2, -2), vec3f(-1, -1, -1)))
        CER_ASSERT_EQ(vec3f(+2, +3, +2), max(vec3f(+2, +2, +2), vec3f(+1, +3, +1)))
        CER_ASSERT_EQ(vec3f(-1, -2, -1), max(vec3f(-2, -2, -2), vec3f(-1, -3, -1)))
        CER_ASSERT_EQ(vec3f(+4, -2, +1), max(vec3f(-2, -2, -2), vec3f(-1, -3, -1), vec3f(4, -4, 1)))
    }

    TEST(vec_test, abs_min) {
        CER_ASSERT_EQ(vec3f(+2, +2, +2), abs_min(vec3f(+2, +2, +2), vec3f(+3, +3, +3)))
        CER_ASSERT_EQ(vec3f(-1, -1, -1), abs_min(vec3f(-2, -2, -2), vec3f(-1, -1, -1)))
        CER_ASSERT_EQ(vec3f(+1, +2, +1), abs_min(vec3f(+2, +2, +2), vec3f(+1, +3, +1)))
        CER_ASSERT_EQ(vec3f(-1, -2, -1), abs_min(vec3f(-2, -2, -2), vec3f(-1, -3, -1)))
        CER_ASSERT_EQ(vec3f( 0,  1, -1), abs_min(vec3f(-2, -2, -2), vec3f(-1, -3, -1), vec3f(0, 1, -4)))
    }

    TEST(vec_test, abs_max) {
        CER_ASSERT_EQ(vec3f(+3, +3, +3), abs_max(vec3f(+2, +2, +2), vec3f(+3, +3, +3)))
        CER_ASSERT_EQ(vec3f(-2, -2, -2), abs_max(vec3f(-2, -2, -2), vec3f(-1, -1, -1)))
        CER_ASSERT_EQ(vec3f(+2, +3, +2), abs_max(vec3f(+2, +2, +2), vec3f(+1, +3, +1)))
        CER_ASSERT_EQ(vec3f(-2, -3, -2), abs_max(vec3f(-2, -2, -2), vec3f(-1, -3, -1)))
        CER_ASSERT_EQ(vec3f(+4, -3, -2), abs_max(vec3f(-2, -2, -2), vec3f(-1, -3, -1), vec3f(+4, -1, 0)))
    }

    TEST(vec_test, abs) {
        CER_ASSERT_EQ(vec3f(1, 2, 3), abs(vec3f(1, -2, -3)))
        CER_ASSERT_EQ(vec3f(0, 2, 3), abs(vec3f(0, -2, -3)))
    }

    TEST(vec_test, sign) {
        CER_ASSERT_EQ(vec3d(+1, +1, +1), sign(vec3d::one()))
        CER_ASSERT_EQ(vec3d( 0,  0,  0), sign(vec3d::zero()))
        CER_ASSERT_EQ(vec3d(-1, -1, -1), sign(-vec3d::one()))
    }

    TEST(vec_test, step) {
        CER_ASSERT_VEC_EQ(vec3d(0, 0, 0), step(+vec3d::one(), vec3d::zero()))
        CER_ASSERT_VEC_EQ(vec3d(1, 1, 1), step(+vec3d::one(), vec3d::one()))
        CER_ASSERT_VEC_EQ(vec3d(0, 0, 1), step(+vec3d::one(), vec3d(-1, 0, 1)))
        CER_ASSERT_VEC_EQ(vec3d(1, 1, 1), step(-vec3d::one(), vec3d(-1, 0, 1)))
        CER_ASSERT_VEC_EQ(vec3d(0, 1, 1), step(-vec3d::one(), vec3d(-2, 0, 1)))
    }

    TEST(vec_test, smoothstep) {
        CER_ASSERT_VEC_EQ(vec3d(0.0,     0.0, 0.0),     smoothstep(vec3d::zero(), vec3d::one(), vec3d(-1.0 , -1.0, -1.0)))
        CER_ASSERT_VEC_EQ(vec3d(0.0,     0.0, 0.0),     smoothstep(vec3d::zero(), vec3d::one(), vec3d( 0.0 ,  0.0,  0.0)))
        CER_ASSERT_VEC_EQ(vec3d(1.0,     1.0, 1.0),     smoothstep(vec3d::zero(), vec3d::one(), vec3d(+1.0 , +1.0, +1.0)))
        CER_ASSERT_VEC_EQ(vec3d(1.0,     1.0, 1.0),     smoothstep(vec3d::zero(), vec3d::one(), vec3d(+2.0 , +2.0, +2.0)))
        CER_ASSERT_VEC_EQ(vec3d(0.0,     0.0, 1.0),     smoothstep(vec3d::zero(), vec3d::one(), vec3d(-1.0 ,  0.0, +2.0)))
        CER_ASSERT_VEC_EQ(vec3d(0.0,     0.5, 1.0),     smoothstep(vec3d::zero(), vec3d::one(), vec3d( 0.0 , +0.5, +1.0)))
        CER_ASSERT_VEC_EQ(vec3d(0.15625, 0.5, 0.84375), smoothstep(vec3d::zero(), vec3d::one(), vec3d(+0.25, +0.5, +0.75)))
    }

    TEST(vec_test, dot) {
        CER_ASSERT_FLOAT_EQ(-748013.6097f, dot(vec3f(2.3f, 8.7878f, -2323.0f), vec3f(4.333f, -2.0f, 322.0f)))
        CER_ASSERT_FLOAT_EQ(0.0f, dot(vec3f(2.3f, 8.7878f, -2323.0f), vec3f::zero()))
    }

    TEST(vec_test, cross) {
        CER_ASSERT_EQ(vec3f::zero(), cross(vec3f::zero(), vec3f::zero()))
        CER_ASSERT_EQ(vec3f::zero(), cross(vec3f::zero(), vec3f(2.0f, 34.233f, -10003.0002f)))
        CER_ASSERT_EQ(vec3f::pos_z(), cross(vec3f::pos_x(), vec3f::pos_y()))
        CER_ASSERT_VEC_EQ(vec3f(-2735141.499f, 282853.508f, 421.138f), cross(vec3f(12.302f, -0.0017f, 79898.3f), vec3f(2.0f, 34.233f, -10003.0002f)))

        constexpr auto t1 = vec3f(7.0f, 4.0f, 0.0f);
        constexpr auto t2 = vec3f(-2.0f, 22.0f, 0.0f);
        CER_ASSERT_VEC_EQ(normalize_c(cross(t1, t2)), normalize_c(cross(normalize_c(t1), normalize_c(t2))))
    }

    TEST(vec_test, squared_length) {
        CER_ASSERT_FLOAT_EQ(0.0f, squared_length(vec3f::zero()))
        CER_ASSERT_FLOAT_EQ(1.0f, squared_length(vec3f::pos_x()))
        CER_ASSERT_FLOAT_EQ(5396411.51542884f, squared_length(vec3f(2.3f, 8.7878f, -2323.0f)))
    }

    TEST(vec_test, length) {
        ASSERT_FLOAT_EQ(0.0f, length(vec3f::zero()));
        ASSERT_FLOAT_EQ(1.0f, length(vec3f::pos_x()));
        ASSERT_FLOAT_EQ(std::sqrt(5396411.51542884f), length(vec3f(2.3f, 8.7878f, -2323.0f)));
    }

    TEST(vec_test, length_c) {
        CE_ASSERT_FLOAT_EQ(0.0f, length_c(vec3f::zero()))
        CE_ASSERT_FLOAT_EQ(1.0f, length_c(vec3f::pos_x()))
        CE_ASSERT_FLOAT_EQ(std::sqrt(5396411.51542884f), length_c(vec3f(2.3f, 8.7878f, -2323.0f)))
    }

    TEST(vec_test, normalize) {
        ASSERT_EQ(vec3f::pos_x(), normalize(vec3f::pos_x()));
        ASSERT_EQ(vec3f::neg_x(), normalize(vec3f::neg_x()));

        const vec3f v1(2.3f, 8.7878f, -2323.0f);
        const vec3f v2(4.333f, -2.0f, 322.0f);
        ASSERT_VEC_EQ((v1 / length(v1)), normalize(v1));
        ASSERT_VEC_EQ((v2 / length(v2)), normalize(v2));
    }

    TEST(vec_test, normalize_c) {
        CE_ASSERT_EQ(vec3f::pos_x(), normalize_c(vec3f::pos_x()))
        CE_ASSERT_EQ(vec3f::neg_x(), normalize_c(vec3f::neg_x()))

        constexpr vec3f v1(2.3f, 8.7878f, -2323.0f);
        constexpr vec3f v2(4.333f, -2.0f, 322.0f);
        CE_ASSERT_VEC_EQ((v1 / length(v1)), normalize_c(v1))
        CE_ASSERT_VEC_EQ((v2 / length(v2)), normalize_c(v2))
    }


    TEST(vec_test, swizzle) {
        CER_ASSERT_EQ(vec3d(2, 3, 1), swizzle(vec3d(1, 2, 3), 0))
        CER_ASSERT_EQ(vec3d(3, 1, 2), swizzle(vec3d(1, 2, 3), 1))
        CER_ASSERT_EQ(vec3d(1, 2, 3), swizzle(vec3d(1, 2, 3), 2))
    }

    TEST(vec_test, unswizzle) {
        CER_ASSERT_EQ(vec3d(1, 2, 3), unswizzle(swizzle(vec3d(1, 2, 3), 0), 0))
        CER_ASSERT_EQ(vec3d(1, 2, 3), unswizzle(swizzle(vec3d(1, 2, 3), 1), 1))
        CER_ASSERT_EQ(vec3d(1, 2, 3), unswizzle(swizzle(vec3d(1, 2, 3), 2), 2))
    }

    TEST(vec_test, is_unit) {
        ASSERT_TRUE(is_unit(vec3f::pos_x(), vm::Cf::almostZero()));
        ASSERT_TRUE(is_unit(vec3f::pos_y(), vm::Cf::almostZero()));
        ASSERT_TRUE(is_unit(vec3f::pos_z(), vm::Cf::almostZero()));
        ASSERT_TRUE(is_unit(vec3f::neg_x(), vm::Cf::almostZero()));
        ASSERT_TRUE(is_unit(vec3f::neg_y(), vm::Cf::almostZero()));
        ASSERT_TRUE(is_unit(vec3f::neg_z(), vm::Cf::almostZero()));
        ASSERT_TRUE(is_unit(normalize(vec3f::one()), vm::Cf::almostZero()));
        ASSERT_FALSE(is_unit(vec3f::one(), vm::Cf::almostZero()));
        ASSERT_FALSE(is_unit(vec3f::zero(), vm::Cf::almostZero()));
    }

    TEST(vec_test, is_unit_c) {
        CE_ASSERT_TRUE(is_unit_c(vec3f::pos_x(), vm::Cf::almostZero()))
        CE_ASSERT_TRUE(is_unit_c(vec3f::pos_y(), vm::Cf::almostZero()))
        CE_ASSERT_TRUE(is_unit_c(vec3f::pos_z(), vm::Cf::almostZero()))
        CE_ASSERT_TRUE(is_unit_c(vec3f::neg_x(), vm::Cf::almostZero()))
        CE_ASSERT_TRUE(is_unit_c(vec3f::neg_y(), vm::Cf::almostZero()))
        CE_ASSERT_TRUE(is_unit_c(vec3f::neg_z(), vm::Cf::almostZero()))
        CE_ASSERT_TRUE(is_unit_c(normalize_c(vec3f::one()), vm::Cf::almostZero()))
        CE_ASSERT_FALSE(is_unit_c(vec3f::one(), vm::Cf::almostZero()))
        CE_ASSERT_FALSE(is_unit_c(vec3f::zero(), vm::Cf::almostZero()))
    }

    TEST(vec_test, is_zero) {
        CER_ASSERT_TRUE(is_zero(vec3f::zero(), vm::Cf::almostZero()))
        CER_ASSERT_FALSE(is_zero(vec3f::pos_x(), vm::Cf::almostZero()))
    }

    TEST(vec_test, is_nan) {
        CER_ASSERT_TRUE(is_nan(vec3f::nan()))
        CER_ASSERT_FALSE(is_nan(vec3f::pos_x()))
    }

    TEST(vec_test, is_integral) {
        CER_ASSERT_TRUE(is_integral(vec3f::pos_x()))
        CER_ASSERT_TRUE(is_integral(vec3f::pos_y()))
        CER_ASSERT_TRUE(is_integral(vec3f::pos_z()))
        CER_ASSERT_TRUE(is_integral(vec3f::neg_x()))
        CER_ASSERT_TRUE(is_integral(vec3f::neg_y()))
        CER_ASSERT_TRUE(is_integral(vec3f::neg_z()))
        CER_ASSERT_TRUE(is_integral(vec3f::one()))
        CER_ASSERT_TRUE(is_integral(vec3f::zero()))
        CER_ASSERT_FALSE(is_integral(normalize_c(vec3f::one())))
    }

    TEST(vec_test, mix) {
        CER_ASSERT_EQ(vec3d::zero(), mix(vec3d::zero(), vec3d::one(), vec3d::zero()))
        CER_ASSERT_EQ(vec3d::one(), mix(vec3d::zero(), vec3d::one(), vec3d::one()))
        CER_ASSERT_EQ(vec3d::one() / 2.0, mix(vec3d::zero(), vec3d::one(), vec3d::one() / 2.0))
    }

    TEST(vec_test, clamp) {
        CER_ASSERT_EQ(vec3d::one(), clamp(vec3d::one(), vec3d::zero(), vec3d(2, 2, 2)))
        CER_ASSERT_EQ(vec3d::one(), clamp(vec3d::one(), vec3d::zero(), vec3d::one()))
        CER_ASSERT_EQ(vec3d::zero(), clamp(vec3d::zero(), vec3d::zero(), vec3d::one()))
        CER_ASSERT_EQ(vec3d(1, 0, 0), clamp(vec3d(2, 0, -1), vec3d::zero(), vec3d::one()))
        CER_ASSERT_EQ(vec3d(2, 0, -1), clamp(vec3d(2, 0, -1), vec3d(1, 0, -2), vec3d(3, 1, 1)))
    }

    TEST(vec_test, fract) {
        CER_ASSERT_VEC_EQ(vec3d::zero(), fract(vec3d::zero()))
        CER_ASSERT_VEC_EQ(vec3d(0.1, 0.7, 0.99999), fract(vec3d(0.1, 0.7, 0.99999)))
        CER_ASSERT_VEC_EQ(vec3d(-0.1, 0.7, -0.99999), fract(vec3d(-0.1, 0.7, -0.99999)))
        CER_ASSERT_VEC_EQ(vec3d(-0.3, 0.7, 0.99999), fract(vec3d(-1.3, 0.7, 1.99999)))
    }

    TEST(vec_test, mod) {
        CER_ASSERT_VEC_EQ(vec3d::zero(), mod(vec3d::one(), vec3d::one()))
        CER_ASSERT_VEC_EQ(vec3d::zero(), mod(vec3d(2, -1, 0), vec3d::one()))
        CER_ASSERT_VEC_EQ(vec3d(0.5, -0.5, 0.5), mod(vec3d(6.5, -6.5, 6.5), vec3d(2, 2, -2)))
    }

    TEST(vec_test, squared_distance) {
        constexpr auto v1 = vec3f(2.3f, 8.7878f, -2323.0f);
        constexpr auto v2 = vec3f(4.333f, -2.0f, 322.0f);

        CER_ASSERT_FLOAT_EQ(0.0f, squared_distance(v1, v1))
        CER_ASSERT_FLOAT_EQ(squared_length(v1), squared_distance(v1, vec3f::zero()))
        CER_ASSERT_FLOAT_EQ(squared_length(v1 - v2), squared_distance(v1, v2))
    }

    TEST(vec_test, distance) {
        constexpr auto v1 = vec3f(2.3f, 8.7878f, -2323.0f);
        constexpr auto v2 = vec3f(4.333f, -2.0f, 322.0f);

        ASSERT_FLOAT_EQ(0.0f, distance(v1, v1));
        ASSERT_FLOAT_EQ(length(v1), distance(v1, vec3f::zero()));
        ASSERT_FLOAT_EQ(length(v1 - v2), distance(v1, v2));
    }

    TEST(vec_test, distance_c) {
        constexpr auto v1 = vec3f(2.3f, 8.7878f, -2323.0f);
        constexpr auto v2 = vec3f(4.333f, -2.0f, 322.0f);

        CE_ASSERT_FLOAT_EQ(0.0f, distance_c(v1, v1))
        CE_ASSERT_FLOAT_EQ(length_c(v1), distance_c(v1, vec3f::zero()))
        CE_ASSERT_FLOAT_EQ(length_c(v1 - v2), distance_c(v1, v2))
    }

    TEST(vec_test, to_homogeneous_coords) {
        CER_ASSERT_EQ(vec4f(1, 2, 3, 1), to_homogeneous_coords(vec3f(1, 2, 3)))
    }

    TEST(vec_test, to_cartesian_coords) {
        constexpr auto v = vec4f(2.0f, 4.0f, 8.0f, 2.0f);
        CER_ASSERT_EQ(vec3f(1.0f, 2.0f, 4.0f), to_cartesian_coords(v))
    }

    TEST(vec_test, is_colinear) {
        CER_ASSERT_TRUE(is_colinear(vec3d::zero(), vec3d::zero(), vec3d::zero()))
        CER_ASSERT_TRUE(is_colinear(vec3d::one(),  vec3d::one(),  vec3d::one()))
        CER_ASSERT_TRUE(is_colinear(vec3d(0.0, 0.0, 0.0), vec3d(0.0, 0.0, 1.0), vec3d(0.0, 0.0, 2.0)))
        CER_ASSERT_FALSE(is_colinear(vec3d(0.0, 0.0, 0.0), vec3d(1.0, 0.0, 0.0), vec3d(0.0, 1.0, 0.0)))
        CER_ASSERT_FALSE(is_colinear(vec3d(0.0, 0.0, 0.0), vec3d(10.0, 0.0, 0.0), vec3d(0.0, 1.0, 0.0)))
    }

    TEST(vec_test, is_parallel) {
        ASSERT_TRUE(is_parallel(vec3f::pos_x(), vec3f::pos_x()));
        ASSERT_TRUE(is_parallel(vec3f::pos_x(), vec3f::neg_x()));
        ASSERT_TRUE(is_parallel(vec3f::one(), vec3f::one()));
        ASSERT_TRUE(is_parallel(vec3f::one(), normalize(vec3f::one())));
    }

    TEST(vec_test, is_parallel_c) {
        CE_ASSERT_TRUE(is_parallel_c(vec3f::pos_x(), vec3f::pos_x()))
        CE_ASSERT_TRUE(is_parallel_c(vec3f::pos_x(), vec3f::neg_x()))
        CE_ASSERT_TRUE(is_parallel_c(vec3f::one(), vec3f::one()))
        CE_ASSERT_TRUE(is_parallel_c(vec3f::one(), normalize_c(vec3f::one())))
    }

    // ========== rounding and error correction ==========

    TEST(vec_test, floor) {
        CER_ASSERT_EQ(vec3f::pos_x(), floor(vec3f::pos_x()))
        CER_ASSERT_EQ(vec3f::one(), floor(vec3f::one()))
        CER_ASSERT_EQ(vec3f::zero(), floor(vec3f::zero()))
        CER_ASSERT_EQ(vec3f::zero(), floor(normalize_c(vec3f::one())))
        CER_ASSERT_EQ(vec3f::zero(), floor(vec3f(0.4, 0.4, 0.4)))
        CER_ASSERT_EQ(vec3f(0, 0, 0), floor(vec3f(0.4, 0.5, 0.4)))
        CER_ASSERT_EQ(vec3f(-1, -1, -1), floor(vec3f(-0.4, -0.5, -0.4)))
    }

    TEST(vec_test, ceil) {
        CER_ASSERT_EQ(vec3f::pos_x(), ceil(vec3f::pos_x()))
        CER_ASSERT_EQ(vec3f::one(), ceil(vec3f::one()))
        CER_ASSERT_EQ(vec3f::zero(), ceil(vec3f::zero()))
        CER_ASSERT_EQ(vec3f::one(), ceil(normalize_c(vec3f::one())))
        CER_ASSERT_EQ(vec3f::one(), ceil(vec3f(0.4, 0.4, 0.4)))
        CER_ASSERT_EQ(vec3f::one(), ceil(vec3f(0.4, 0.5, 0.4)))
        CER_ASSERT_EQ(vec3f::zero(), ceil(vec3f(-0.4, -0.5, -0.4)))
        CER_ASSERT_EQ(vec3f(-1, -1, -1), ceil(vec3f(-1.4, -1.5, -1.4)))
    }

    TEST(vec_test, trunc) {
        CER_ASSERT_EQ(vec3f::pos_x(), trunc(vec3f::pos_x()))
        CER_ASSERT_EQ(vec3f::one(), trunc(vec3f::one()))
        CER_ASSERT_EQ(vec3f::zero(), trunc(vec3f::zero()))
        CER_ASSERT_EQ(vec3f::zero(), trunc(normalize_c(vec3f::one())))
        CER_ASSERT_EQ(vec3f::zero(), trunc(normalize_c(-vec3f::one())))
        CER_ASSERT_EQ(vec3f::zero(), trunc(vec3f(0.4, 0.4, 0.4)))
        CER_ASSERT_EQ(vec3f::zero(), trunc(vec3f(0.4, 0.5, 0.4)))
        CER_ASSERT_EQ(vec3f::zero(), trunc(vec3f(-0.4, -0.5, -0.4)))
        CER_ASSERT_EQ(vec3f(-1, -1, -1), trunc(vec3f(-1.4, -1.5, -1.4)))
    }

    TEST(vec_test, round) {
        CER_ASSERT_EQ(vec3f::pos_x(), round(vec3f::pos_x()))
        CER_ASSERT_EQ(vec3f::one(), round(vec3f::one()))
        CER_ASSERT_EQ(vec3f::zero(), round(vec3f::zero()))
        CER_ASSERT_EQ(vec3f::one(), round(normalize_c(vec3f::one())))
        CER_ASSERT_EQ(vec3f::zero(), round(vec3f(0.4, 0.4, 0.4)))
        CER_ASSERT_EQ(vec3f(0, 1, 0), round(vec3f(0.4, 0.5, 0.4)))
        CER_ASSERT_EQ(vec3f(0, -1, 0), round(vec3f(-0.4, -0.5, -0.4)))
    }

    TEST(vec_test, snapDown) {
        CER_ASSERT_EQ( vec3f::zero(), snapDown(vec3f::zero(), vec3f::one()))
        CER_ASSERT_EQ( vec3f::zero(), snapDown(vec3f(+0.4, +0.5, +0.6), vec3f::one()))
        CER_ASSERT_EQ( vec3f::zero(), snapDown(vec3f(-0.4, -0.5, -0.6), vec3f::one()))
        CER_ASSERT_EQ(+vec3f::one(),  snapDown(vec3f(+1.4, +1.5, +1.6), vec3f::one()))
        CER_ASSERT_EQ(-vec3f::one(),  snapDown(vec3f(-1.4, -1.5, -1.6), vec3f::one()))
        CER_ASSERT_EQ( vec3f::zero(), snapDown(vec3f(+1.4, +1.5, +1.6), vec3f(2, 2, 2)))
        CER_ASSERT_EQ( vec3f::zero(), snapDown(vec3f(-1.4, -1.5, -1.6), vec3f(2, 2, 2)))
        CER_ASSERT_EQ( vec3f(0, +1, +1), snapDown(vec3f(+1.4, +1.5, +1.6), vec3f(2, 1, 1)))
        CER_ASSERT_EQ( vec3f(0, -1, -1), snapDown(vec3f(-1.4, -1.5, -1.6), vec3f(2, 1, 1)))
    }

    TEST(vec_test, snapUp) {
        CER_ASSERT_EQ( vec3f::zero(), snapUp(vec3f::zero(), vec3f::one()))
        CER_ASSERT_EQ(+vec3f::one(),  snapUp(vec3f(+0.4, +0.5, +0.6), vec3f::one()))
        CER_ASSERT_EQ(-vec3f::one(),  snapUp(vec3f(-0.4, -0.5, -0.6), vec3f::one()))
        CER_ASSERT_EQ(+vec3f(+2, +2, +2), snapUp(vec3f(+1.4, +1.5, +1.6), vec3f::one()))
        CER_ASSERT_EQ(-vec3f(+2, +2, +2), snapUp(vec3f(-1.4, -1.5, -1.6), vec3f::one()))
        CER_ASSERT_EQ( vec3f(+3, +3, +3), snapUp(vec3f(+1.4, +1.5, +1.6), vec3f(3, 3, 3)))
        CER_ASSERT_EQ( vec3f(-3, -3, -3), snapUp(vec3f(-1.4, -1.5, -1.6), vec3f(3, 3, 3)))
        CER_ASSERT_EQ( vec3f(+3, +2, +2), snapUp(vec3f(+1.4, +1.5, +1.6), vec3f(3, 1, 1)))
        CER_ASSERT_EQ( vec3f(-3, -2, -2), snapUp(vec3f(-1.4, -1.5, -1.6), vec3f(3, 1, 1)))
    }

    TEST(vec_test, snap) {
        CER_ASSERT_EQ(vec2f( 8.0f,  0.0f), snap(vec2f( 7.0f, -3.0f), vec2f( 4.0f, 12.0f)))
        CER_ASSERT_EQ(vec2f( 8.0f, -6.0f), snap(vec2f( 7.0f, -5.0f), vec2f(-4.0f, -2.0f)))
        CER_ASSERT_EQ(vec2f(-8.0f,  6.0f), snap(vec2f(-7.0f,  5.0f), vec2f(-4.0f, -2.0f)))
    }

    TEST(vec_test, correct) {
        CER_ASSERT_EQ(vec3f(1.1, 2.2, 3.3), correct(vec3f(1.1, 2.2, 3.3)))
        CER_ASSERT_EQ(vec3f(1, 2, 3), correct(vec3f(1.1, 2.2, 3.3), 0, 0.4f))
        CER_ASSERT_EQ(vec3f(1.1, 2.2, 3.3), correct(vec3f(1.1, 2.2, 3.3), 1, 0.4f))
    }

    TEST(vec_test, is_between) {
        ASSERT_TRUE(is_between(vec3f(1, 0, 0), vec3f(0, 0, 0), vec3f(2, 0, 0)));
        ASSERT_TRUE(is_between(vec3f(1, 0, 0), vec3f(2, 0, 0), vec3f(0, 0, 0)));
        ASSERT_TRUE(is_between(vec3f(1, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 0)));
        ASSERT_TRUE(is_between(vec3f(0, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 0)));
        ASSERT_FALSE(is_between(vec3f(2, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 0)));
    }

    TEST(vec_test, is_between_c) {
        CE_ASSERT_TRUE(is_between_c(vec3f(1, 0, 0), vec3f(0, 0, 0), vec3f(2, 0, 0)))
        CE_ASSERT_TRUE(is_between_c(vec3f(1, 0, 0), vec3f(2, 0, 0), vec3f(0, 0, 0)))
        CE_ASSERT_TRUE(is_between_c(vec3f(1, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 0)))
        CE_ASSERT_TRUE(is_between_c(vec3f(0, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 0)))
        CE_ASSERT_FALSE(is_between_c(vec3f(2, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 0)))
    }

    TEST(vec_test, average) {
        constexpr auto vecs = std::array { vec3f(1, 1, 1), vec3f(1, 1, 1), vec3f(2, 2, 2) };
        CER_ASSERT_EQ(vec3f(4.0 / 3.0, 4.0 / 3.0, 4.0 / 3.0), average(std::begin(vecs), std::end(vecs)))
    }

    TEST(vec_test, measure_angle) {
        ASSERT_FLOAT_EQ(0.0f, measure_angle(vec3f::pos_x(), vec3f::pos_x(), vec3f::pos_z()));
        ASSERT_FLOAT_EQ(Cf::piOverTwo(), measure_angle(vec3f::pos_y(), vec3f::pos_x(), vec3f::pos_z()));
        ASSERT_FLOAT_EQ(Cf::pi(), measure_angle(vec3f::neg_x(), vec3f::pos_x(), vec3f::pos_z()));
        ASSERT_FLOAT_EQ(3.0f * Cf::piOverTwo(), measure_angle(vec3f::neg_y(), vec3f::pos_x(), vec3f::pos_z()));
    }
}
