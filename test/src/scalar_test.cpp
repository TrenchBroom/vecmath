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

#include <vecmath/scalar.h>

#include <array>
#include <limits>

namespace vm {
    TEST(scalar_test, identity) {
        constexpr auto id = identity();
        CER_ASSERT_EQ(1, id(1))
        CER_ASSERT_EQ(-1, id(-1))
        CER_ASSERT_DOUBLE_EQ(1.234, id(1.234))
    }

    TEST(scalar_test, is_nan) {
        CER_ASSERT_TRUE(is_nan(std::numeric_limits<double>::quiet_NaN()))
        CER_ASSERT_TRUE(is_nan(std::numeric_limits<float>::quiet_NaN()))
        CER_ASSERT_FALSE(is_nan(1.0))
        CER_ASSERT_FALSE(is_nan(1.0f))
    }

    TEST(scalar_test, is_inf) {
        CER_ASSERT_TRUE(is_inf(+std::numeric_limits<double>::infinity()))
        CER_ASSERT_TRUE(is_inf(-std::numeric_limits<double>::infinity()))
        CER_ASSERT_TRUE(is_inf(+std::numeric_limits<float>::infinity()))
        CER_ASSERT_TRUE(is_inf(-std::numeric_limits<float>::infinity()))
        CER_ASSERT_FALSE(is_inf(0.0))
        CER_ASSERT_FALSE(is_inf(0.0f))
    }

    TEST(scalar_test, nan) {
        CER_ASSERT_TRUE(is_nan(nan<double>()))
        CER_ASSERT_TRUE(is_nan(nan<float>()))
    }

    TEST(scalar_test, min) {
        CER_ASSERT_EQ(+1.0, min(+1.0, +1.0))
        CER_ASSERT_EQ(+1.0, min(+1.0, +2.0))
        CER_ASSERT_EQ(+1.0, min(+2.0, +1.0))
        CER_ASSERT_EQ(-1.0, min(-1.0, +2.0))
        CER_ASSERT_EQ(-2.0, min(+1.0, -2.0))
        CER_ASSERT_EQ(-2.0, min(-1.0, -2.0))
        CER_ASSERT_EQ(-3.0, min(-1.0, -2.0, -3.0))
        CER_ASSERT_EQ(-3.0, min(-1.0, -3.0, -2.0))
        CER_ASSERT_EQ(-3.0, min(-2.0, -1.0, -3.0))
        CER_ASSERT_EQ(-3.0, min(-2.0, -3.0, -1.0))
        CER_ASSERT_EQ(-3.0, min(-3.0, -1.0, -2.0))
        CER_ASSERT_EQ(-3.0, min(-3.0, -2.0, -1.0))
    }

    TEST(scalar_test, max) {
        CER_ASSERT_EQ(+1.0, max(+1.0, +1.0))
        CER_ASSERT_EQ(+2.0, max(+1.0, +2.0))
        CER_ASSERT_EQ(+2.0, max(+2.0, +1.0))
        CER_ASSERT_EQ(+2.0, max(-1.0, +2.0))
        CER_ASSERT_EQ(+1.0, max(+1.0, -2.0))
        CER_ASSERT_EQ(-1.0, max(-1.0, -2.0))
        CER_ASSERT_EQ(-1.0, max(-1.0, -2.0, -3.0))
        CER_ASSERT_EQ(-1.0, max(-1.0, -3.0, -2.0))
        CER_ASSERT_EQ(-1.0, max(-2.0, -1.0, -3.0))
        CER_ASSERT_EQ(-1.0, max(-2.0, -3.0, -1.0))
        CER_ASSERT_EQ(-1.0, max(-3.0, -1.0, -2.0))
        CER_ASSERT_EQ(-1.0, max(-3.0, -2.0, -1.0))
    }

    TEST(scalar_test, abs_min) {
        CER_ASSERT_EQ(+1.0, abs_min(+1.0, +1.0))
        CER_ASSERT_EQ(+1.0, abs_min(+1.0, +2.0))
        CER_ASSERT_EQ(+1.0, abs_min(+2.0, +1.0))
        CER_ASSERT_EQ(-1.0, abs_min(-1.0, +2.0))
        CER_ASSERT_EQ(+1.0, abs_min(+1.0, -2.0))
        CER_ASSERT_EQ(-1.0, abs_min(-1.0, -2.0))
        CER_ASSERT_EQ(+1.0, abs_min(+1.0, -2.0, +3.0))
    }

    TEST(scalar_test, abs_max) {
        CER_ASSERT_EQ(+1.0, abs_max(+1.0, +1.0))
        CER_ASSERT_EQ(+2.0, abs_max(+1.0, +2.0))
        CER_ASSERT_EQ(+2.0, abs_max(+2.0, +1.0))
        CER_ASSERT_EQ(+2.0, abs_max(-1.0, +2.0))
        CER_ASSERT_EQ(-2.0, abs_max(+1.0, -2.0))
        CER_ASSERT_EQ(-2.0, abs_max(-1.0, -2.0))
        CER_ASSERT_EQ(-3.0, abs_max(-1.0, -2.0, -3.0))
    }

    TEST(scalar_test, safe_min) {
        CER_ASSERT_EQ(+1.0, safe_min(+1.0, +1.0))
        CER_ASSERT_EQ(+1.0, safe_min(+1.0, +2.0))
        CER_ASSERT_EQ(+1.0, safe_min(+2.0, +1.0))
        CER_ASSERT_EQ(-1.0, safe_min(-1.0, +2.0))
        CER_ASSERT_EQ(-2.0, safe_min(+1.0, -2.0))
        CER_ASSERT_EQ(-2.0, safe_min(-1.0, -2.0))
        CER_ASSERT_EQ(-3.0, safe_min(-1.0, -2.0, -3.0))

        CER_ASSERT_EQ(+1.0, safe_min(+1.0, nan<double>()))
        CER_ASSERT_EQ(-1.0, safe_min(nan<double>(), -1.0))
        CER_ASSERT_TRUE(is_nan(safe_min(nan<double>(), nan<double>())))

        CER_ASSERT_EQ(-2.0, safe_min(nan<double>(), +1.0, -2.0))
        CER_ASSERT_EQ(-2.0, safe_min(+1.0, nan<double>(), -2.0))
        CER_ASSERT_EQ(-2.0, safe_min(+1.0, -2.0, nan<double>()))
        CER_ASSERT_EQ(+1.0, safe_min(+1.0, nan<double>(), nan<double>()))
        CER_ASSERT_TRUE(is_nan(safe_min(nan<double>(), nan<double>(), nan<double>())))
    }

    TEST(scalar_test, safe_max) {
        CER_ASSERT_EQ(+1.0, safe_max(+1.0, +1.0))
        CER_ASSERT_EQ(+2.0, safe_max(+1.0, +2.0))
        CER_ASSERT_EQ(+2.0, safe_max(+2.0, +1.0))
        CER_ASSERT_EQ(+2.0, safe_max(-1.0, +2.0))
        CER_ASSERT_EQ(+1.0, safe_max(+1.0, -2.0))
        CER_ASSERT_EQ(-1.0, safe_max(-1.0, -2.0))

        CER_ASSERT_EQ(+1.0, safe_max(+1.0, nan<double>()))
        CER_ASSERT_EQ(-1.0, safe_max(nan<double>(), -1.0))
        CER_ASSERT_TRUE(is_nan(safe_max(nan<double>(), nan<double>())))

        CER_ASSERT_EQ(+1.0, safe_max(nan<double>(), +1.0, -2.0))
        CER_ASSERT_EQ(+1.0, safe_max(+1.0, nan<double>(), -2.0))
        CER_ASSERT_EQ(+1.0, safe_max(+1.0, -2.0, nan<double>()))
        CER_ASSERT_EQ(+1.0, safe_max(+1.0, nan<double>(), nan<double>()))
        CER_ASSERT_TRUE(is_nan(safe_max(nan<double>(), nan<double>(), nan<double>())))
    }

    TEST(scalar_test, abs_difference) {
        CER_ASSERT_EQ(3, abs_difference(+4, +7))
        CER_ASSERT_EQ(3, abs_difference(+7, +4))
        CER_ASSERT_EQ(6, abs_difference(+7, -1))
        CER_ASSERT_EQ(6, abs_difference(-7, +1))
        CER_ASSERT_EQ(6, abs_difference(-7, -1))
    }

    TEST(scalar_test, clamp) {
        CER_ASSERT_EQ( 0.0, clamp( 0.0,  0.0, +1.0))
        CER_ASSERT_EQ(+1.0, clamp(+1.0,  0.0, +1.0))
        CER_ASSERT_EQ( 0.0, clamp(-1.0,  0.0, +1.0))
        CER_ASSERT_EQ(+1.0, clamp(+2.0,  0.0, +1.0))
        CER_ASSERT_EQ(+0.5, clamp(+0.5,  0.0, +1.0))

        CER_ASSERT_EQ( 0.0, clamp( 0.0, -1.0,  0.0))
        CER_ASSERT_EQ(-1.0, clamp(-1.0, -1.0,  0.0))
        CER_ASSERT_EQ( 0.0, clamp(+1.0, -1.0,  0.0))
        CER_ASSERT_EQ(-1.0, clamp(-2.0, -1.0,  0.0))
        CER_ASSERT_EQ(-0.5, clamp(-0.5, -1.0,  0.0))

        CER_ASSERT_EQ( 0.0, clamp( 0.0, -1.0, +1.0))
        CER_ASSERT_EQ(-1.0, clamp(-1.0, -1.0, +1.0))
        CER_ASSERT_EQ(+1.0, clamp(+1.0, -1.0, +1.0))
        CER_ASSERT_EQ(-1.0, clamp(-2.0, -1.0, +1.0))
        CER_ASSERT_EQ(+1.0, clamp(+2.0, -1.0, +1.0))
    }

    TEST(scalar_test, sign) {
        CER_ASSERT_EQ(-1, sign(-2))
        CER_ASSERT_EQ(-1, sign(-1))
        CER_ASSERT_EQ( 0, sign( 0))
        CER_ASSERT_EQ(+1, sign( 1))
        CER_ASSERT_EQ(+1, sign( 2))
    }

    TEST(scalar_test, step) {
        CER_ASSERT_EQ(0, step(1, -1))
        CER_ASSERT_EQ(0, step(1,  0))
        CER_ASSERT_EQ(1, step(1,  1))
        CER_ASSERT_EQ(1, step(1,  2))
    }

    TEST(scalar_test, smoothstep) {
        CER_ASSERT_DOUBLE_EQ(0.0,     smoothstep(0.0, 1.0, -1.0))
        CER_ASSERT_DOUBLE_EQ(0.0,     smoothstep(0.0, 1.0,  0.0))
        CER_ASSERT_DOUBLE_EQ(0.15625, smoothstep(0.0, 1.0,  0.25))
        CER_ASSERT_DOUBLE_EQ(0.5,     smoothstep(0.0, 1.0,  0.5))
        CER_ASSERT_DOUBLE_EQ(0.84375, smoothstep(0.0, 1.0,  0.75))
        CER_ASSERT_DOUBLE_EQ(1.0,     smoothstep(0.0, 1.0,  1.0))
        CER_ASSERT_DOUBLE_EQ(1.0,     smoothstep(0.0, 1.0,  2.0))
    }

    TEST(scalar_test, mod) {
        CER_ASSERT_DOUBLE_EQ( 0.0, mod(+4.0, +2.0))
        CER_ASSERT_DOUBLE_EQ(+1.0, mod(+5.0, +2.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, mod(-5.0, +2.0))
        CER_ASSERT_DOUBLE_EQ(+1.0, mod(+5.0, -2.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, mod(-5.0, -2.0))
        CER_ASSERT_DOUBLE_EQ(+1.5, mod(+5.5, +2.0))
    }

    TEST(scalar_test, floor) {
        CER_ASSERT_DOUBLE_EQ(-1.0, floor(-0.7))
        CER_ASSERT_DOUBLE_EQ(-1.0, floor(-0.5))
        CER_ASSERT_DOUBLE_EQ(-1.0, floor(-0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, floor( 0.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, floor( 0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, floor( 0.6))
        CER_ASSERT_DOUBLE_EQ( 1.0, floor( 1.0))
    }

    TEST(scalar_test, ceil) {
        CER_ASSERT_DOUBLE_EQ(-1.0, ceil(-1.1))
        CER_ASSERT_DOUBLE_EQ( 0.0, ceil(-0.7))
        CER_ASSERT_DOUBLE_EQ( 0.0, ceil(-0.5))
        CER_ASSERT_DOUBLE_EQ( 0.0, ceil(-0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, ceil( 0.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, ceil( 0.4))
        CER_ASSERT_DOUBLE_EQ( 1.0, ceil( 0.6))
        CER_ASSERT_DOUBLE_EQ( 1.0, ceil( 1.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, ceil( 1.1))
    }

    TEST(scalar_test, trunc) {
        CER_ASSERT_DOUBLE_EQ(-1.0, trunc(-1.1))
        CER_ASSERT_DOUBLE_EQ( 0.0, trunc(-0.7))
        CER_ASSERT_DOUBLE_EQ( 0.0, trunc(-0.5))
        CER_ASSERT_DOUBLE_EQ( 0.0, trunc(-0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, trunc( 0.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, trunc( 0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, trunc( 0.6))
        CER_ASSERT_DOUBLE_EQ(+1.0, trunc( 1.0))
        CER_ASSERT_DOUBLE_EQ(+1.0, trunc( 1.1))
    }

    TEST(scalar_test, mix) {
        CER_ASSERT_DOUBLE_EQ(1.0, mix(1.0, 2.0, 0.0))
        CER_ASSERT_DOUBLE_EQ(2.0, mix(1.0, 2.0, 1.0))
        CER_ASSERT_DOUBLE_EQ(1.5, mix(1.0, 2.0, 0.5))

        CER_ASSERT_DOUBLE_EQ(-1.0, mix(-1.0, 2.0, 0.0))
        CER_ASSERT_DOUBLE_EQ(+2.0, mix(-1.0, 2.0, 1.0))
        CER_ASSERT_DOUBLE_EQ(+0.5, mix(-1.0, 2.0, 0.5))

        CER_ASSERT_DOUBLE_EQ(-1.0, mix(-1.0, -2.0, 0.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, mix(-1.0, -2.0, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.5, mix(-1.0, -2.0, 0.5))
    }

    TEST(scalar_test, fract) {
        CER_ASSERT_DOUBLE_EQ(-0.2, fract(-1.2))
        CER_ASSERT_DOUBLE_EQ( 0.0, fract(-1.0))
        CER_ASSERT_DOUBLE_EQ(-0.7, fract(-0.7))
        CER_ASSERT_DOUBLE_EQ( 0.0, fract( 0.0))
        CER_ASSERT_DOUBLE_EQ(+0.7, fract(+0.7))
        CER_ASSERT_DOUBLE_EQ( 0.0, fract(+1.0))
        CER_ASSERT_DOUBLE_EQ( 0.2, fract(+1.2))
    }

    TEST(scalar_test, round) {
        CER_ASSERT_DOUBLE_EQ(-1.0, round(-1.1))
        CER_ASSERT_DOUBLE_EQ(-1.0, round(-0.7))
        CER_ASSERT_DOUBLE_EQ(-1.0, round(-0.5))
        CER_ASSERT_DOUBLE_EQ( 0.0, round(-0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, round( 0.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, round( 0.4))
        CER_ASSERT_DOUBLE_EQ(+1.0, round( 0.6))
        CER_ASSERT_DOUBLE_EQ(+1.0, round( 1.0))
        CER_ASSERT_DOUBLE_EQ(+1.0, round( 1.1))
    }

    TEST(scalar_test, round_up) {
        CER_ASSERT_DOUBLE_EQ(-2.0, round_up(-1.1))
        CER_ASSERT_DOUBLE_EQ(-1.0, round_up(-0.7))
        CER_ASSERT_DOUBLE_EQ(-1.0, round_up(-0.5))
        CER_ASSERT_DOUBLE_EQ(-1.0, round_up(-0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_up(0.0))
        CER_ASSERT_DOUBLE_EQ(+1.0, round_up(0.4))
        CER_ASSERT_DOUBLE_EQ(+1.0, round_up(0.6))
        CER_ASSERT_DOUBLE_EQ(+1.0, round_up(1.0))
        CER_ASSERT_DOUBLE_EQ(+2.0, round_up(1.1))
    }

    TEST(scalar_test, round_down) {
        CER_ASSERT_DOUBLE_EQ(-1.0, round_down(-1.1))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_down(-0.7))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_down(-0.5))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_down(-0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_down(0.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_down(0.4))
        CER_ASSERT_DOUBLE_EQ( 0.0, round_down(0.6))
        CER_ASSERT_DOUBLE_EQ(+1.0, round_down(1.0))
        CER_ASSERT_DOUBLE_EQ(+1.0, round_down(1.1))
    }

    TEST(scalar_test, snap) {
        CER_ASSERT_DOUBLE_EQ( 0.0, snap( 0.0, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(+0.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snap(+0.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snap(+0.6, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(-0.4, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snap(-0.5, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snap(-0.6, 1.0))

        CER_ASSERT_DOUBLE_EQ( 1.0, snap(+1.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snap(+1.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snap(+1.6, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snap(-1.4, 1.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snap(-1.5, 1.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snap(-1.6, 1.0))

        CER_ASSERT_DOUBLE_EQ( 0.0, snap( 0.0, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(+0.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(+0.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(+0.6, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(-0.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(-0.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snap(-0.6, 2.0))

        CER_ASSERT_DOUBLE_EQ( 2.0, snap(+1.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snap(+1.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snap(+1.6, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snap(-1.4, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snap(-1.5, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snap(-1.6, 2.0))
    }

    TEST(scalar_test, snapUp) {
        CER_ASSERT_DOUBLE_EQ( 0.0, snapUp( 0.0, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snapUp(+0.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snapUp(+0.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snapUp(+0.6, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snapUp(-0.4, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snapUp(-0.5, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snapUp(-0.6, 1.0))

        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.6, 1.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.4, 1.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.5, 1.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.6, 1.0))

        CER_ASSERT_DOUBLE_EQ( 0.0, snapUp( 0.0, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+0.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+0.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+0.6, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-0.4, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-0.5, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-0.6, 2.0))

        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.6, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.4, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.5, 2.0))
        CER_ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.6, 2.0))
    }

    TEST(scalar_test, snapDown) {
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown( 0.0, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.6, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.6, 1.0))

        CER_ASSERT_DOUBLE_EQ( 1.0, snapDown(+1.4, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snapDown(+1.5, 1.0))
        CER_ASSERT_DOUBLE_EQ( 1.0, snapDown(+1.6, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snapDown(-1.4, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snapDown(-1.5, 1.0))
        CER_ASSERT_DOUBLE_EQ(-1.0, snapDown(-1.6, 1.0))

        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown( 0.0, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.6, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.6, 2.0))

        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+1.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+1.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(+1.6, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-1.4, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-1.5, 2.0))
        CER_ASSERT_DOUBLE_EQ( 0.0, snapDown(-1.6, 2.0))
    }

    TEST(scalar_test, correct) {
        CER_ASSERT_DOUBLE_EQ(+1.1, correct(+1.1))

        CER_ASSERT_DOUBLE_EQ(+1.0, correct(+1.1, 0, 0.4))
        CER_ASSERT_DOUBLE_EQ(-1.0, correct(-1.1, 0, 0.4))
        CER_ASSERT_DOUBLE_EQ(+1.0, correct(+1.3, 0, 0.4))
        CER_ASSERT_DOUBLE_EQ(+1.4, correct(+1.4, 0, 0.3))

        CER_ASSERT_DOUBLE_EQ(+1.1, correct(+1.1, 1, 0.4))
        CER_ASSERT_DOUBLE_EQ(-1.1, correct(-1.1, 1, 0.4))
        CER_ASSERT_DOUBLE_EQ(+1.3, correct(+1.3, 1, 0.4))
        CER_ASSERT_DOUBLE_EQ(+1.4, correct(+1.4, 1, 0.3))
    }

    TEST(scalar_test, is_equal) {
        CER_ASSERT_TRUE(is_equal(+1.0, +1.0, 0.0))
        CER_ASSERT_TRUE(is_equal(-1.0, -1.0, 0.0))
        CER_ASSERT_TRUE(is_equal(-1.001, -1.001, 0.0))
        CER_ASSERT_TRUE(is_equal(+1.0, +1.001, 0.1))
        CER_ASSERT_TRUE(is_equal(+1.0, +1.0999, 0.1))

        CER_ASSERT_FALSE(is_equal(+1.0, +1.11, 0.1))
        CER_ASSERT_FALSE(is_equal(+1.0, +1.1, 0.09))
        CER_ASSERT_FALSE(is_equal(-1.0, +1.11, 0.1))
        CER_ASSERT_FALSE(is_equal(+1.0, +1.1, 0.0))
    }

    TEST(scalar_test, is_zero) {
        CER_ASSERT_TRUE(is_zero(0.0, 0.0))
        CER_ASSERT_TRUE(is_zero(0.0, 0.1))
        CER_ASSERT_TRUE(is_zero(0.099, 0.1))
        CER_ASSERT_TRUE(is_zero(-0.099, 0.1))
        CER_ASSERT_FALSE(is_zero(0.099, 0.0))
        CER_ASSERT_FALSE(is_zero(-1.0, 0.0))
    }

    TEST(scalar_test, contains) {
        CER_ASSERT_TRUE(contains(0.0, 0.0, 1.0))
        CER_ASSERT_TRUE(contains(1.0, 0.0, 1.0))
        CER_ASSERT_TRUE(contains(0.0, 1.0, 0.0))
        CER_ASSERT_TRUE(contains(1.0, 1.0, 0.0))

        CER_ASSERT_FALSE(contains(+1.1, 0.0, 1.0))
        CER_ASSERT_FALSE(contains(+1.1, 1.0, 0.0))
        CER_ASSERT_FALSE(contains(-0.1, 0.0, 1.0))
        CER_ASSERT_FALSE(contains(-0.1, 1.0, 0.0))
    }

    TEST(scalar_test, to_radians) {
        using c = constants<double>;

        CER_ASSERT_EQ(0.0, to_radians(0.0))
        CER_ASSERT_EQ(c::half_pi(), to_radians(90.0))
        CER_ASSERT_EQ(c::pi(), to_radians(180.0))
        CER_ASSERT_EQ(c::two_pi(), to_radians(360.0))
        CER_ASSERT_EQ(-c::pi(), to_radians(-180.0))
        CER_ASSERT_EQ(-c::two_pi(), to_radians(-360.0))
    }

    TEST(scalar_test, to_degrees) {
        using c = constants<double>;

        CER_ASSERT_EQ(0.0, to_degrees(0.0))
        CER_ASSERT_EQ(90.0, to_degrees(c::half_pi()))
        CER_ASSERT_EQ(180.0, to_degrees(c::pi()))
        CER_ASSERT_EQ(360.0, to_degrees(c::two_pi()))
        CER_ASSERT_EQ(-180.0, to_degrees(-c::pi()))
        CER_ASSERT_EQ(-360.0, to_degrees(-c::two_pi()))
    }

    TEST(scalar_test, normalize_radians) {
        using c = constants<double>;

        CER_ASSERT_EQ(0.0, normalize_radians(c::two_pi()))
        CER_ASSERT_EQ(c::half_pi(), normalize_radians(c::half_pi()))
        CER_ASSERT_EQ(c::three_half_pi(), normalize_radians(-c::half_pi()))
        CER_ASSERT_EQ(c::half_pi(), normalize_radians(c::half_pi() + c::two_pi()))
    }

    TEST(scalar_test, normalize_degrees) {
        CER_ASSERT_EQ(0.0, normalize_degrees(0.0))
        CER_ASSERT_EQ(0.0, normalize_degrees(360.0))
        CER_ASSERT_EQ(90.0, normalize_degrees(90.0))
        CER_ASSERT_EQ(270.0, normalize_degrees(-90.0))
        CER_ASSERT_EQ(90.0, normalize_degrees(360.0 + 90.0))
    }

    TEST(scalar_test, succ) {
        CER_ASSERT_EQ(0u, succ(0u, 1u))
        CER_ASSERT_EQ(1u, succ(0u, 2u))
        CER_ASSERT_EQ(0u, succ(1u, 2u))
        CER_ASSERT_EQ(2u, succ(0u, 3u, 2u))
        CER_ASSERT_EQ(1u, succ(2u, 3u, 2u))
    }

    TEST(scalar_test, pred) {
        CER_ASSERT_EQ(0u, pred(0u, 1u))
        CER_ASSERT_EQ(1u, pred(0u, 2u))
        CER_ASSERT_EQ(0u, pred(1u, 2u))
        CER_ASSERT_EQ(1u, pred(0u, 3u, 2u))
        CER_ASSERT_EQ(0u, pred(2u, 3u, 2u))
    }

    TEST(scalar_test, nextgreater) {
        ASSERT_TRUE(+1.0 < nextgreater(+1.0));
        ASSERT_TRUE(-1.0 < nextgreater(-1.0));
    }

#define ASSERT_SQRT(v) ASSERT_DOUBLE_EQ(std::sqrt((v)), sqrt((v)));

    TEST(scalar_test, sqrt) {
        for (double v = 0.0; v < 20.0; v += 0.1) {
            ASSERT_SQRT(v)
        }

        ASSERT_TRUE(is_nan(sqrt(nan<double>())));
        ASSERT_TRUE(is_nan(sqrt(-1.0)));
        ASSERT_SQRT(std::numeric_limits<double>::infinity());
    }

#define CE_ASSERT_SQRT(v) CE_ASSERT_DOUBLE_EQ(std::sqrt((v)), sqrt_c((v)))

    TEST(scalar_test, sqrt_c) {
        CE_ASSERT_SQRT(0.0)
        CE_ASSERT_SQRT(0.2)
        CE_ASSERT_SQRT(1.0)
        CE_ASSERT_SQRT(2.0)
        CE_ASSERT_SQRT(4.0)
        CE_ASSERT_SQRT(5.2)
        CE_ASSERT_SQRT(5.2394839489348)
        CE_ASSERT_SQRT(223235.2394839489348)
        CE_ASSERT_SQRT(std::numeric_limits<double>::infinity())

        CE_ASSERT_TRUE(is_nan(sqrt_c(nan<double>())))
        CE_ASSERT_TRUE(is_nan(sqrt_c(-1.0)))
    }

    template <typename T>
    void assertSolution(const std::tuple<std::size_t, T, T>& expected, const std::tuple<std::size_t, T, T>& actual);

    template <typename T>
    void assertSolution(const std::tuple<std::size_t, T, T, T>& expected, const std::tuple<std::size_t, T, T, T>& actual);

    template <typename T>
    void assertSolution(const std::tuple<std::size_t, T, T, T, T>& expected, const std::tuple<std::size_t, T, T, T, T>& actual);

    TEST(scalar_test, solve_quadratic) {
        using c = constants<double>;

        assertSolution(
            { 2, 2.0, -8.0 },
            solve_quadratic(1.0, 6.0, -16.0, c::almost_zero())
        );
        assertSolution(
            { 2, -1.0, -9.0 },
            solve_quadratic(1.0, 10.0, 9.0, c::almost_zero())
        );
        assertSolution(
            { 2, 7.0, -4.0 },
            solve_quadratic(0.5, -1.5, -14.0, c::almost_zero())
        );
        assertSolution(
            { 1, 2.0, nan<double>() },
            solve_quadratic(1.0, -4.0, 4.0, c::almost_zero())
        );
        assertSolution(
            { 0, nan<double>(), nan<double>() },
            solve_quadratic(1.0, 12.0, 37.0, c::almost_zero())
        );
    }

    TEST(scalar_test, solve_cubic) {
        using c = constants<double>;

        assertSolution(
            { 1, -2.0, nan<double>(), nan<double>() },
            solve_cubic(1.0, 0.0, -2.0, 4.0, c::almost_zero())
        );
        assertSolution(
            { 1, 7.0 / 9.0, nan<double>(), nan<double>() },
            solve_cubic(9.0, -43.0, 145.0, -91.0, c::almost_zero())
        );
        assertSolution(
            { 3, 4.464101615, 2.0, -2.464101615 },
            solve_cubic(1.0, -4.0, -7.0, 22.0, c::almost_zero())
        );


        // casus irreducibilis
        assertSolution(
            { 2, -2.0, 1.0, nan<double>() },
            solve_cubic(1.0, 0.0, -3.0, 2.0, c::almost_zero())
        );
        assertSolution(
            { 3, 4.0 / 3.0, 1.0 / 3.0, -10.0 / 6.0 },
            solve_cubic(1.0, 0.0, -7.0 / 3.0, 20.0 / 27.0, c::almost_zero())
        );
    }

    TEST(scalar_test, solve_quartic) {
        using c = constants<double>;

        assertSolution(
            { 0, nan<double>(), nan<double>(), nan<double>(), nan<double>() },
            solve_quartic(1.0, 1.0, 1.0, 1.0, 1.0, c::almost_zero())
        );
        assertSolution(
             { 0, nan<double>(), nan<double>(), nan<double>(), nan<double>() },
            solve_quartic(1.0, -1.0, 1.0, -1.0, 1.0, c::almost_zero())
        );
        assertSolution(
             { 4, -0.203258341626567109, -4.91984728399109344, 2.76090563295441601, 0.362199992663244539 },
            solve_quartic(1.0, 2.0, -14.0, 2.0, 1.0, c::almost_zero())
        );
        assertSolution(
             { 2, 1.5986745079, -1.0, nan<double>(), nan<double>() },
            solve_quartic(1.0, 3.0, 0.0, -8.0, -6.0, c::almost_zero())
        );
        assertSolution(
             { 2, -1.0, -1.0, nan<double>(), nan<double>() },
            solve_quartic(1.0, 4.0, 6.0, 4.0, 1.0, c::almost_zero())
        );
        assertSolution(
             { 2, -3.0, 2.0, nan<double>(), nan<double>() },
            solve_quartic(1.0, 2.0, -11.0, -12.0, 36.0, c::almost_zero())
        );
        assertSolution(
            { 4,
                -1.0 - sqrt(6.0),
                -1.0 - sqrt(11.0),
                sqrt(11.0) - 1.0,
                sqrt(6.0) - 1.0
            },
            solve_quartic(1.0, 4.0, -11.0, -30.0, 50.0, c::almost_zero())
        );
    }


    template <typename T>
    void assertSolution(const std::tuple<std::size_t, T, T>& expected, const std::tuple<std::size_t, T, T>& actual) {
        const auto expectedNum = std::get<0>(expected);
        const auto actualNum   = std::get<0>(actual);

        ASSERT_EQ(expectedNum, actualNum);
        if (expectedNum > 0) {
            ASSERT_NEAR(std::get<1>(expected), std::get<1>(actual), 0.00000001);
        }
        if (expectedNum > 1) {
            ASSERT_NEAR(std::get<2>(expected), std::get<2>(actual), 0.00000001);
        }
    }

    template <typename T>
    void assertSolution(const std::tuple<std::size_t, T, T, T>& expected, const std::tuple<std::size_t, T, T, T>& actual) {
        assertSolution<T>(
            { std::get<0>(expected), std::get<1>(expected), std::get<2>(expected) },
            { std::get<0>(actual),   std::get<1>(actual),   std::get<2>(actual) } );

        if (std::get<0>(expected) > 2) {
            ASSERT_NEAR(std::get<3>(expected), std::get<3>(actual), 0.00000001);
        }
    }

    template <typename T>
    void assertSolution(const std::tuple<std::size_t, T, T, T, T>& expected, const std::tuple<std::size_t, T, T, T, T>& actual) {
        assertSolution<T>(
            { std::get<0>(expected), std::get<1>(expected), std::get<2>(expected), std::get<3>(expected) },
            { std::get<0>(actual),   std::get<1>(actual),   std::get<2>(actual),   std::get<3>(actual) } );

        if (std::get<0>(expected) > 3) {
            ASSERT_NEAR(std::get<4>(expected), std::get<4>(actual), 0.00000001);
        }
    }
}
