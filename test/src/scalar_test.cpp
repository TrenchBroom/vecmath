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

#include <vecmath/scalar.h>

#include <array>
#include <limits>

namespace vm {
    TEST(scalar_test, identity) {
        constexpr auto id = identity();
        CA_ASSERT_EQ(1, id(1))
        CA_ASSERT_EQ(-1, id(-1))
        CA_ASSERT_DOUBLE_EQ(1.234, id(1.234))
    }

    TEST(scalar_test, is_nan) {
        CA_ASSERT_TRUE(is_nan(std::numeric_limits<double>::quiet_NaN()))
        CA_ASSERT_TRUE(is_nan(std::numeric_limits<float>::quiet_NaN()))
        CA_ASSERT_FALSE(is_nan(1.0))
        CA_ASSERT_FALSE(is_nan(1.0f))
    }

    TEST(scalar_test, is_inf) {
        CA_ASSERT_TRUE(is_inf(+std::numeric_limits<double>::infinity()))
        CA_ASSERT_TRUE(is_inf(-std::numeric_limits<double>::infinity()))
        CA_ASSERT_TRUE(is_inf(+std::numeric_limits<float>::infinity()))
        CA_ASSERT_TRUE(is_inf(-std::numeric_limits<float>::infinity()))
        CA_ASSERT_FALSE(is_inf(0.0))
        CA_ASSERT_FALSE(is_inf(0.0f))
    }

    TEST(scalar_test, nan) {
        CA_ASSERT_TRUE(is_nan(nan<double>()))
        CA_ASSERT_TRUE(is_nan(nan<float>()))
    }

    TEST(scalar_test, min) {
        CA_ASSERT_EQ(+1.0, min(+1.0, +1.0))
        CA_ASSERT_EQ(+1.0, min(+1.0, +2.0))
        CA_ASSERT_EQ(+1.0, min(+2.0, +1.0))
        CA_ASSERT_EQ(-1.0, min(-1.0, +2.0))
        CA_ASSERT_EQ(-2.0, min(+1.0, -2.0))
        CA_ASSERT_EQ(-2.0, min(-1.0, -2.0))
    }

    TEST(scalar_test, max) {
        // constexpr evaluation
        const auto result = max(0.0, 1.0);
        ASSERT_EQ(1.0, result);

        ASSERT_EQ(+1.0, max(+1.0, +1.0));
        ASSERT_EQ(+2.0, max(+1.0, +2.0));
        ASSERT_EQ(+2.0, max(+2.0, +1.0));
        ASSERT_EQ(+2.0, max(-1.0, +2.0));
        ASSERT_EQ(+1.0, max(+1.0, -2.0));
        ASSERT_EQ(-1.0, max(-1.0, -2.0));
    }

    TEST(scalar_test, abs_min) {
        constexpr auto result = abs_min(+1.0, +1.0);
        ASSERT_EQ(+1.0, result);

        ASSERT_EQ(+1.0, abs_min(+1.0, +1.0));
        ASSERT_EQ(+1.0, abs_min(+1.0, +2.0));
        ASSERT_EQ(+1.0, abs_min(+2.0, +1.0));
        ASSERT_EQ(-1.0, abs_min(-1.0, +2.0));
        ASSERT_EQ(+1.0, abs_min(+1.0, -2.0));
        ASSERT_EQ(-1.0, abs_min(-1.0, -2.0));
    }

    TEST(scalar_test, abs_max) {
        constexpr auto result = abs_max(+1.0, +1.0);
        ASSERT_EQ(+1.0, result);

        ASSERT_EQ(+1.0, abs_max(+1.0, +1.0));
        ASSERT_EQ(+2.0, abs_max(+1.0, +2.0));
        ASSERT_EQ(+2.0, abs_max(+2.0, +1.0));
        ASSERT_EQ(+2.0, abs_max(-1.0, +2.0));
        ASSERT_EQ(-2.0, abs_max(+1.0, -2.0));
        ASSERT_EQ(-2.0, abs_max(-1.0, -2.0));
    }

    TEST(scalar_test, safe_min) {
        constexpr auto result = safe_min(+1.0, +1.0);
        ASSERT_EQ(+1.0, result);

        ASSERT_EQ(+1.0, safe_min(+1.0, +1.0));
        ASSERT_EQ(+1.0, safe_min(+1.0, +2.0));
        ASSERT_EQ(+1.0, safe_min(+2.0, +1.0));
        ASSERT_EQ(-1.0, safe_min(-1.0, +2.0));
        ASSERT_EQ(-2.0, safe_min(+1.0, -2.0));
        ASSERT_EQ(-2.0, safe_min(-1.0, -2.0));

        ASSERT_EQ(+1.0, safe_min(+1.0, nan<double>()));
        ASSERT_EQ(-1.0, safe_min(nan<double>(), -1.0));
        ASSERT_TRUE(is_nan(safe_min(nan<double>(), nan<double>())));
    }

    TEST(scalar_test, safe_max) {
        constexpr auto result = safe_max(+1.0, +1.0);
        ASSERT_EQ(+1.0, result);

        ASSERT_EQ(+1.0, safe_max(+1.0, +1.0));
        ASSERT_EQ(+2.0, safe_max(+1.0, +2.0));
        ASSERT_EQ(+2.0, safe_max(+2.0, +1.0));
        ASSERT_EQ(+2.0, safe_max(-1.0, +2.0));
        ASSERT_EQ(+1.0, safe_max(+1.0, -2.0));
        ASSERT_EQ(-1.0, safe_max(-1.0, -2.0));

        ASSERT_EQ(+1.0, safe_max(+1.0, nan<double>()));
        ASSERT_EQ(-1.0, safe_max(nan<double>(), -1.0));
        ASSERT_TRUE(is_nan(safe_max(nan<double>(), nan<double>())));
    }

    TEST(scalar_test, abs_difference) {
        constexpr auto result = abs_difference(+4, +7);
        ASSERT_EQ(3, result);

        ASSERT_EQ(3, abs_difference(+4, +7));
        ASSERT_EQ(3, abs_difference(+7, +4));
        ASSERT_EQ(6, abs_difference(+7, -1));
        ASSERT_EQ(6, abs_difference(-7, +1));
        ASSERT_EQ(6, abs_difference(-7, -1));
    }

    TEST(scalar_test, clamp) {
        constexpr auto result = clamp( 0.0,  0.0, +1.0);
        ASSERT_EQ( 0.0, result);

        ASSERT_EQ( 0.0, clamp( 0.0,  0.0, +1.0));
        ASSERT_EQ(+1.0, clamp(+1.0,  0.0, +1.0));
        ASSERT_EQ( 0.0, clamp(-1.0,  0.0, +1.0));
        ASSERT_EQ(+1.0, clamp(+2.0,  0.0, +1.0));
        ASSERT_EQ(+0.5, clamp(+0.5,  0.0, +1.0));

        ASSERT_EQ( 0.0, clamp( 0.0, -1.0,  0.0));
        ASSERT_EQ(-1.0, clamp(-1.0, -1.0,  0.0));
        ASSERT_EQ( 0.0, clamp(+1.0, -1.0,  0.0));
        ASSERT_EQ(-1.0, clamp(-2.0, -1.0,  0.0));
        ASSERT_EQ(-0.5, clamp(-0.5, -1.0,  0.0));

        ASSERT_EQ( 0.0, clamp( 0.0, -1.0, +1.0));
        ASSERT_EQ(-1.0, clamp(-1.0, -1.0, +1.0));
        ASSERT_EQ(+1.0, clamp(+1.0, -1.0, +1.0));
        ASSERT_EQ(-1.0, clamp(-2.0, -1.0, +1.0));
        ASSERT_EQ(+1.0, clamp(+2.0, -1.0, +1.0));
    }

    TEST(scalar_test, sign) {
        constexpr auto result = sign(-2);
        ASSERT_EQ(-1, result);

        ASSERT_EQ(-1, sign(-2));
        ASSERT_EQ(-1, sign(-1));
        ASSERT_EQ( 0, sign( 0));
        ASSERT_EQ(+1, sign( 1));
        ASSERT_EQ(+1, sign( 2));
    }

    TEST(scalar_test, step) {
        constexpr auto result = step(1, -1);
        ASSERT_EQ(0, result);

        ASSERT_EQ(0, step(1, -1));
        ASSERT_EQ(0, step(1,  0));
        ASSERT_EQ(1, step(1,  1));
        ASSERT_EQ(1, step(1,  2));
    }

    TEST(scalar_test, smoothstep) {
        constexpr auto result = smoothstep(0.0, 1.0, -1.0);
        ASSERT_DOUBLE_EQ(0.0, result);

        ASSERT_DOUBLE_EQ(0.0,     smoothstep(0.0, 1.0, -1.0));
        ASSERT_DOUBLE_EQ(0.0,     smoothstep(0.0, 1.0,  0.0));
        ASSERT_DOUBLE_EQ(0.15625, smoothstep(0.0, 1.0,  0.25));
        ASSERT_DOUBLE_EQ(0.5,     smoothstep(0.0, 1.0,  0.5));
        ASSERT_DOUBLE_EQ(0.84375, smoothstep(0.0, 1.0,  0.75));
        ASSERT_DOUBLE_EQ(1.0,     smoothstep(0.0, 1.0,  1.0));
        ASSERT_DOUBLE_EQ(1.0,     smoothstep(0.0, 1.0,  2.0));
    }

    TEST(scalar_test, mod) {
        constexpr auto result = mod(+4.0, +2.0);
        ASSERT_DOUBLE_EQ( 0.0, result);

        ASSERT_DOUBLE_EQ( 0.0, mod(+4.0, +2.0));
        ASSERT_DOUBLE_EQ(+1.0, mod(+5.0, +2.0));
        ASSERT_DOUBLE_EQ(-1.0, mod(-5.0, +2.0));
        ASSERT_DOUBLE_EQ(+1.0, mod(+5.0, -2.0));
        ASSERT_DOUBLE_EQ(-1.0, mod(-5.0, -2.0));
        ASSERT_DOUBLE_EQ(+1.5, mod(+5.5, +2.0));
    }

    TEST(scalar_test, floor) {
        constexpr auto result = floor(-0.7);
        ASSERT_DOUBLE_EQ(-1.0, result);

        ASSERT_DOUBLE_EQ(-1.0, floor(-0.7));
        ASSERT_DOUBLE_EQ(-1.0, floor(-0.5));
        ASSERT_DOUBLE_EQ(-1.0, floor(-0.4));
        ASSERT_DOUBLE_EQ( 0.0, floor( 0.0));
        ASSERT_DOUBLE_EQ( 0.0, floor( 0.4));
        ASSERT_DOUBLE_EQ( 0.0, floor( 0.6));
        ASSERT_DOUBLE_EQ( 1.0, floor( 1.0));
    }

    TEST(scalar_test, ceil) {
        constexpr auto result = ceil(-1.1);
        ASSERT_DOUBLE_EQ(-1.0, result);

        ASSERT_DOUBLE_EQ(-1.0, ceil(-1.1));
        ASSERT_DOUBLE_EQ( 0.0, ceil(-0.7));
        ASSERT_DOUBLE_EQ( 0.0, ceil(-0.5));
        ASSERT_DOUBLE_EQ( 0.0, ceil(-0.4));
        ASSERT_DOUBLE_EQ( 0.0, ceil( 0.0));
        ASSERT_DOUBLE_EQ( 1.0, ceil( 0.4));
        ASSERT_DOUBLE_EQ( 1.0, ceil( 0.6));
        ASSERT_DOUBLE_EQ( 1.0, ceil( 1.0));
        ASSERT_DOUBLE_EQ( 2.0, ceil( 1.1));
    }

    TEST(scalar_test, trunc) {
        constexpr auto result = trunc(-1.1);
        ASSERT_DOUBLE_EQ(-1.0, result);

        ASSERT_DOUBLE_EQ(-1.0, trunc(-1.1));
        ASSERT_DOUBLE_EQ( 0.0, trunc(-0.7));
        ASSERT_DOUBLE_EQ( 0.0, trunc(-0.5));
        ASSERT_DOUBLE_EQ( 0.0, trunc(-0.4));
        ASSERT_DOUBLE_EQ( 0.0, trunc( 0.0));
        ASSERT_DOUBLE_EQ( 0.0, trunc( 0.4));
        ASSERT_DOUBLE_EQ( 0.0, trunc( 0.6));
        ASSERT_DOUBLE_EQ(+1.0, trunc( 1.0));
        ASSERT_DOUBLE_EQ(+1.0, trunc( 1.1));
    }

    TEST(scalar_test, mix) {
        constexpr auto result = mix(1.0, 2.0, 0.0);
        ASSERT_DOUBLE_EQ(1.0, result);

        ASSERT_DOUBLE_EQ(1.0, mix(1.0, 2.0, 0.0));
        ASSERT_DOUBLE_EQ(2.0, mix(1.0, 2.0, 1.0));
        ASSERT_DOUBLE_EQ(1.5, mix(1.0, 2.0, 0.5));

        ASSERT_DOUBLE_EQ(-1.0, mix(-1.0, 2.0, 0.0));
        ASSERT_DOUBLE_EQ(+2.0, mix(-1.0, 2.0, 1.0));
        ASSERT_DOUBLE_EQ(+0.5, mix(-1.0, 2.0, 0.5));

        ASSERT_DOUBLE_EQ(-1.0, mix(-1.0, -2.0, 0.0));
        ASSERT_DOUBLE_EQ(-2.0, mix(-1.0, -2.0, 1.0));
        ASSERT_DOUBLE_EQ(-1.5, mix(-1.0, -2.0, 0.5));
    }

    TEST(scalar_test, fract) {
        constexpr auto result = fract(-1.2);
        ASSERT_DOUBLE_EQ(-0.2, result);

        ASSERT_DOUBLE_EQ(-0.2, fract(-1.2));
        ASSERT_DOUBLE_EQ( 0.0, fract(-1.0));
        ASSERT_DOUBLE_EQ(-0.7, fract(-0.7));
        ASSERT_DOUBLE_EQ( 0.0, fract( 0.0));
        ASSERT_DOUBLE_EQ(+0.7, fract(+0.7));
        ASSERT_DOUBLE_EQ( 0.0, fract(+1.0));
        ASSERT_DOUBLE_EQ( 0.2, fract(+1.2));
    }

    TEST(scalar_test, round) {
        constexpr auto result = round(-1.1);
        ASSERT_DOUBLE_EQ(-1.0, result);

        ASSERT_DOUBLE_EQ(-1.0, round(-1.1));
        ASSERT_DOUBLE_EQ(-1.0, round(-0.7));
        ASSERT_DOUBLE_EQ(-1.0, round(-0.5));
        ASSERT_DOUBLE_EQ( 0.0, round(-0.4));
        ASSERT_DOUBLE_EQ( 0.0, round( 0.0));
        ASSERT_DOUBLE_EQ( 0.0, round( 0.4));
        ASSERT_DOUBLE_EQ(+1.0, round( 0.6));
        ASSERT_DOUBLE_EQ(+1.0, round( 1.0));
        ASSERT_DOUBLE_EQ(+1.0, round( 1.1));
    }

    TEST(scalar_test, round_up) {
        constexpr auto result = round_up(-1.1);
        ASSERT_DOUBLE_EQ(-2.0, result);

        ASSERT_DOUBLE_EQ(-2.0, round_up(-1.1));
        ASSERT_DOUBLE_EQ(-1.0, round_up(-0.7));
        ASSERT_DOUBLE_EQ(-1.0, round_up(-0.5));
        ASSERT_DOUBLE_EQ(-1.0, round_up(-0.4));
        ASSERT_DOUBLE_EQ( 0.0, round_up(0.0));
        ASSERT_DOUBLE_EQ(+1.0, round_up(0.4));
        ASSERT_DOUBLE_EQ(+1.0, round_up(0.6));
        ASSERT_DOUBLE_EQ(+1.0, round_up(1.0));
        ASSERT_DOUBLE_EQ(+2.0, round_up(1.1));
    }

    TEST(scalar_test, round_down) {
        constexpr auto result = round_down(-1.1);
        ASSERT_DOUBLE_EQ(-1.0, result);

        ASSERT_DOUBLE_EQ(-1.0, round_down(-1.1));
        ASSERT_DOUBLE_EQ( 0.0, round_down(-0.7));
        ASSERT_DOUBLE_EQ( 0.0, round_down(-0.5));
        ASSERT_DOUBLE_EQ( 0.0, round_down(-0.4));
        ASSERT_DOUBLE_EQ( 0.0, round_down(0.0));
        ASSERT_DOUBLE_EQ( 0.0, round_down(0.4));
        ASSERT_DOUBLE_EQ( 0.0, round_down(0.6));
        ASSERT_DOUBLE_EQ(+1.0, round_down(1.0));
        ASSERT_DOUBLE_EQ(+1.0, round_down(1.1));
    }

    TEST(scalar_test, snap) {
        constexpr auto result = snap(0.0, 1.0);
        ASSERT_DOUBLE_EQ( 0.0, result);

        ASSERT_DOUBLE_EQ( 0.0, snap( 0.0, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(+0.4, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snap(+0.5, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snap(+0.6, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(-0.4, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snap(-0.5, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snap(-0.6, 1.0));

        ASSERT_DOUBLE_EQ( 1.0, snap(+1.4, 1.0));
        ASSERT_DOUBLE_EQ( 2.0, snap(+1.5, 1.0));
        ASSERT_DOUBLE_EQ( 2.0, snap(+1.6, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snap(-1.4, 1.0));
        ASSERT_DOUBLE_EQ(-2.0, snap(-1.5, 1.0));
        ASSERT_DOUBLE_EQ(-2.0, snap(-1.6, 1.0));

        ASSERT_DOUBLE_EQ( 0.0, snap( 0.0, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(+0.4, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(+0.5, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(+0.6, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(-0.4, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(-0.5, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snap(-0.6, 2.0));

        ASSERT_DOUBLE_EQ( 2.0, snap(+1.4, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snap(+1.5, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snap(+1.6, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snap(-1.4, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snap(-1.5, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snap(-1.6, 2.0));
    }

    TEST(scalar_test, snapUp) {
        constexpr auto result = snapUp(0.0, 1.0);
        ASSERT_DOUBLE_EQ( 0.0, result);

        ASSERT_DOUBLE_EQ( 0.0, snapUp( 0.0, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snapUp(+0.4, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snapUp(+0.5, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snapUp(+0.6, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snapUp(-0.4, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snapUp(-0.5, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snapUp(-0.6, 1.0));

        ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.4, 1.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.5, 1.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.6, 1.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.4, 1.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.5, 1.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.6, 1.0));

        ASSERT_DOUBLE_EQ( 0.0, snapUp( 0.0, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+0.4, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+0.5, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+0.6, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-0.4, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-0.5, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-0.6, 2.0));

        ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.4, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.5, 2.0));
        ASSERT_DOUBLE_EQ( 2.0, snapUp(+1.6, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.4, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.5, 2.0));
        ASSERT_DOUBLE_EQ(-2.0, snapUp(-1.6, 2.0));
    }

    TEST(scalar_test, snapDown) {
        constexpr auto result = snapDown(0.0, 1.0);
        ASSERT_DOUBLE_EQ( 0.0, result);

        ASSERT_DOUBLE_EQ( 0.0, snapDown( 0.0, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.4, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.5, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.6, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.4, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.5, 1.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.6, 1.0));

        ASSERT_DOUBLE_EQ( 1.0, snapDown(+1.4, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snapDown(+1.5, 1.0));
        ASSERT_DOUBLE_EQ( 1.0, snapDown(+1.6, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snapDown(-1.4, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snapDown(-1.5, 1.0));
        ASSERT_DOUBLE_EQ(-1.0, snapDown(-1.6, 1.0));

        ASSERT_DOUBLE_EQ( 0.0, snapDown( 0.0, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.4, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.5, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+0.6, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.4, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.5, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-0.6, 2.0));

        ASSERT_DOUBLE_EQ( 0.0, snapDown(+1.4, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+1.5, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(+1.6, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-1.4, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-1.5, 2.0));
        ASSERT_DOUBLE_EQ( 0.0, snapDown(-1.6, 2.0));
    }

    TEST(scalar_test, correct) {
        constexpr auto result = correct(+1.1);
        ASSERT_DOUBLE_EQ(+1.1, result);

        ASSERT_DOUBLE_EQ(+1.1, correct(+1.1));

        ASSERT_DOUBLE_EQ(+1.0, correct(+1.1, 0, 0.4));
        ASSERT_DOUBLE_EQ(-1.0, correct(-1.1, 0, 0.4));
        ASSERT_DOUBLE_EQ(+1.0, correct(+1.3, 0, 0.4));
        ASSERT_DOUBLE_EQ(+1.4, correct(+1.4, 0, 0.3));

        ASSERT_DOUBLE_EQ(+1.1, correct(+1.1, 1, 0.4));
        ASSERT_DOUBLE_EQ(-1.1, correct(-1.1, 1, 0.4));
        ASSERT_DOUBLE_EQ(+1.3, correct(+1.3, 1, 0.4));
        ASSERT_DOUBLE_EQ(+1.4, correct(+1.4, 1, 0.3));
    }

    TEST(scalar_test, is_equal) {
        constexpr auto result = is_equal(+1.0, +1.0, 0.0);
        ASSERT_TRUE(result);

        ASSERT_TRUE(is_equal(+1.0, +1.0, 0.0));
        ASSERT_TRUE(is_equal(-1.0, -1.0, 0.0));
        ASSERT_TRUE(is_equal(-1.001, -1.001, 0.0));
        ASSERT_TRUE(is_equal(+1.0, +1.001, 0.1));
        ASSERT_TRUE(is_equal(+1.0, +1.0999, 0.1));

        ASSERT_FALSE(is_equal(+1.0, +1.11, 0.1));
        ASSERT_FALSE(is_equal(+1.0, +1.1, 0.09));
        ASSERT_FALSE(is_equal(-1.0, +1.11, 0.1));
        ASSERT_FALSE(is_equal(+1.0, +1.1, 0.0));
    }

    TEST(scalar_test, is_zero) {
        constexpr auto result = is_zero(0.0, 0.0);
        ASSERT_TRUE(result);

        ASSERT_TRUE(is_zero(0.0, 0.0));
        ASSERT_TRUE(is_zero(0.0, 0.1));
        ASSERT_TRUE(is_zero(0.099, 0.1));
        ASSERT_TRUE(is_zero(-0.099, 0.1));
        ASSERT_FALSE(is_zero(0.099, 0.0));
        ASSERT_FALSE(is_zero(-1.0, 0.0));
    }

    TEST(scalar_test, contains) {
        constexpr auto result = contains(0.0, 0.0, 1.0);
        ASSERT_TRUE(result);

        ASSERT_TRUE(contains(0.0, 0.0, 1.0));
        ASSERT_TRUE(contains(1.0, 0.0, 1.0));
        ASSERT_TRUE(contains(0.0, 1.0, 0.0));
        ASSERT_TRUE(contains(1.0, 1.0, 0.0));

        ASSERT_FALSE(contains(+1.1, 0.0, 1.0));
        ASSERT_FALSE(contains(+1.1, 1.0, 0.0));
        ASSERT_FALSE(contains(-0.1, 0.0, 1.0));
        ASSERT_FALSE(contains(-0.1, 1.0, 0.0));
    }

    TEST(scalar_test, to_radians) {
        using c = constants<double>;

        constexpr auto result = to_radians(0.0);
        ASSERT_EQ(0.0, result);

        ASSERT_EQ(0.0, to_radians(0.0));
        ASSERT_EQ(c::piOverTwo(), to_radians(90.0));
        ASSERT_EQ(c::pi(), to_radians(180.0));
        ASSERT_EQ(c::twoPi(), to_radians(360.0));
        ASSERT_EQ(-c::pi(), to_radians(-180.0));
        ASSERT_EQ(-c::twoPi(), to_radians(-360.0));
    }

    TEST(scalar_test, to_degrees) {
        using c = constants<double>;

        constexpr auto result = to_degrees(0.0);
        ASSERT_EQ(0.0, result);

        ASSERT_EQ(0.0, to_degrees(0.0));
        ASSERT_EQ(90.0, to_degrees(c::piOverTwo()));
        ASSERT_EQ(180.0, to_degrees(c::pi()));
        ASSERT_EQ(360.0, to_degrees(c::twoPi()));
        ASSERT_EQ(-180.0, to_degrees(-c::pi()));
        ASSERT_EQ(-360.0, to_degrees(-c::twoPi()));
    }

    TEST(scalar_test, normalize_radians) {
        using c = constants<double>;

        constexpr auto result = normalize_radians(0.0);
        ASSERT_EQ(0.0, result);

        ASSERT_EQ(0.0, normalize_radians(c::twoPi()));
        ASSERT_EQ(c::piOverTwo(), normalize_radians(c::piOverTwo()));
        ASSERT_EQ(c::threePiOverTwo(), normalize_radians(-c::piOverTwo()));
        ASSERT_EQ(c::piOverTwo(), normalize_radians(c::piOverTwo() + c::twoPi()));
    }

    TEST(scalar_test, normalize_degrees) {
        constexpr auto result = normalize_degrees(0.0);
        ASSERT_EQ(0.0, result);

        ASSERT_EQ(0.0, normalize_degrees(0.0));
        ASSERT_EQ(0.0, normalize_degrees(360.0));
        ASSERT_EQ(90.0, normalize_degrees(90.0));
        ASSERT_EQ(270.0, normalize_degrees(-90.0));
        ASSERT_EQ(90.0, normalize_degrees(360.0 + 90.0));
    }

    TEST(scalar_test, succ) {
        constexpr auto result = succ(0u, 1u);
        ASSERT_EQ(0u, result);

        ASSERT_EQ(0u, succ(0u, 1u));
        ASSERT_EQ(1u, succ(0u, 2u));
        ASSERT_EQ(0u, succ(1u, 2u));
        ASSERT_EQ(2u, succ(0u, 3u, 2u));
        ASSERT_EQ(1u, succ(2u, 3u, 2u));
    }

    TEST(scalar_test, pred) {
        constexpr auto result = pred(0u, 1u);
        ASSERT_EQ(0u, result);

        ASSERT_EQ(0u, pred(0u, 1u));
        ASSERT_EQ(1u, pred(0u, 2u));
        ASSERT_EQ(0u, pred(1u, 2u));
        ASSERT_EQ(1u, pred(0u, 3u, 2u));
        ASSERT_EQ(0u, pred(2u, 3u, 2u));
    }

    TEST(scalar_test, nextgreater) {
        ASSERT_TRUE(+1.0 < nextgreater(+1.0));
        ASSERT_TRUE(-1.0 < nextgreater(-1.0));
    }

    template <size_t S>
    void assertSolution(const std::tuple<size_t, std::array<double,S>>& expected, const std::tuple<size_t, std::array<double,S>>& actual);

    TEST(scalar_test, solveQuadratic) {
        using c = constants<double>;

        assertSolution(
            { 2, { 2.0, -8.0 } },
            solveQuadratic(1.0, 6.0, -16.0, c::almostZero())
        );
        assertSolution(
            { 2, { -1.0, -9.0 } },
            solveQuadratic(1.0, 10.0, 9.0, c::almostZero())
        );
        assertSolution(
            { 2, { 7.0, -4.0 } },
            solveQuadratic(0.5, -1.5, -14.0, c::almostZero())
        );
        assertSolution(
            { 1, { 2.0, nan<double>() } },
            solveQuadratic(1.0, -4.0, 4.0, c::almostZero())
        );
        assertSolution(
            { 0, { nan<double>(), nan<double>() } },
            solveQuadratic(1.0, 12.0, 37.0, c::almostZero())
        );
    }

    TEST(scalar_test, solveCubic) {
        using c = constants<double>;

        assertSolution(
            { 1, { -2.0, nan<double>(), nan<double>() } },
            solveCubic(1.0, 0.0, -2.0, 4.0, c::almostZero())
        );
        assertSolution(
            { 1, { 7.0 / 9.0, nan<double>(), nan<double>() } },
            solveCubic(9.0, -43.0, 145.0, -91.0, c::almostZero())
        );
        assertSolution(
            { 3, { 4.464101615, 2.0, -2.464101615 } },
            solveCubic(1.0, -4.0, -7.0, 22.0, c::almostZero())
        );


        // casus irreducibilis
        assertSolution(
            { 2, { -2.0, 1.0, nan<double>() } },
            solveCubic(1.0, 0.0, -3.0, 2.0, c::almostZero())
        );
        assertSolution(
            { 3, { 4.0 / 3.0, 1.0 / 3.0, -10.0 / 6.0 } },
            solveCubic(1.0, 0.0, -7.0 / 3.0, 20.0 / 27.0, c::almostZero())
        );
    }

    TEST(scalar_test, solveQuartic) {
        using c = constants<double>;

        assertSolution(
            { 0, { nan<double>(), nan<double>(), nan<double>(), nan<double>() } },
            solveQuartic(1.0, 1.0, 1.0, 1.0, 1.0, c::almostZero())
        );
        assertSolution(
            { 0, { nan<double>(), nan<double>(), nan<double>(), nan<double>() } },
            solveQuartic(1.0, -1.0, 1.0, -1.0, 1.0, c::almostZero())
        );
        assertSolution(
            { 4, { -0.203258341626567109, -4.91984728399109344, 2.76090563295441601, 0.362199992663244539 } },
            solveQuartic(1.0, 2.0, -14.0, 2.0, 1.0, c::almostZero())
        );
        assertSolution(
            { 2, { 1.5986745079, -1.0, nan<double>(), nan<double>() } },
            solveQuartic(1.0, 3.0, 0.0, -8.0, -6.0, c::almostZero())
        );
        assertSolution(
            { 2, { -1.0, -1.0, nan<double>(), nan<double>() } },
            solveQuartic(1.0, 4.0, 6.0, 4.0, 1.0, c::almostZero())
        );
        assertSolution(
            { 2, { -3.0, 2.0, nan<double>(), nan<double>() } },
            solveQuartic(1.0, 2.0, -11.0, -12.0, 36.0, c::almostZero())
        );
        assertSolution(
            { 4, {
                -1.0 - std::sqrt(6.0),
                -1.0 - std::sqrt(11.0),
                std::sqrt(11.0) - 1.0,
                std::sqrt(6.0) - 1.0
            } },
            solveQuartic(1.0, 4.0, -11.0, -30.0, 50.0, c::almostZero())
        );
    }

    template <size_t S>
    void assertSolution(const std::tuple<size_t, std::array<double,S>>& expected, const std::tuple<size_t, std::array<double,S>>& actual) {
        const auto [expectedNum, expectedSol] = expected;
        const auto [actualNum, actualSol] = actual;

        ASSERT_EQ(expectedNum, actualNum);
        for (size_t i = 0; i < expectedNum; ++i) {
            ASSERT_NEAR(expectedSol[i], actualSol[i], 0.00000001);
        }
    }
}
