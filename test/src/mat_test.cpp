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
#include <vecmath/mat.h>
#include <vecmath/vec.h>
// #include <vecmath/quat.h>

#include "test_utils.h"

#include <cstdlib>
#include <ctime>

namespace vm {
    TEST(mat_test, constructor_default) {
        CER_ASSERT_MAT_EQ(mat4x4d::identity(), mat4x4d());
    }

    TEST(mat_test, constructor_initializer_list) {
        constexpr auto m1 = mat3x3d({
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        });
        CER_ASSERT_DOUBLE_EQ(1.0, m1[0][0])
        CER_ASSERT_DOUBLE_EQ(2.0, m1[1][0])
        CER_ASSERT_DOUBLE_EQ(3.0, m1[2][0])
        CER_ASSERT_DOUBLE_EQ(4.0, m1[0][1])
        CER_ASSERT_DOUBLE_EQ(5.0, m1[1][1])
        CER_ASSERT_DOUBLE_EQ(6.0, m1[2][1])
        CER_ASSERT_DOUBLE_EQ(7.0, m1[0][2])
        CER_ASSERT_DOUBLE_EQ(8.0, m1[1][2])
        CER_ASSERT_DOUBLE_EQ(9.0, m1[2][2])

        constexpr auto m2 = mat<double, 4, 3>({
             1.0,  2.0,  3.0,
             4.0,  5.0,  6.0,
             7.0,  8.0,  9.0,
            10.0, 11.0, 12.0
        });
        CER_ASSERT_DOUBLE_EQ( 1.0, m2[0][0])
        CER_ASSERT_DOUBLE_EQ( 2.0, m2[1][0])
        CER_ASSERT_DOUBLE_EQ( 3.0, m2[2][0])
        CER_ASSERT_DOUBLE_EQ( 4.0, m2[0][1])
        CER_ASSERT_DOUBLE_EQ( 5.0, m2[1][1])
        CER_ASSERT_DOUBLE_EQ( 6.0, m2[2][1])
        CER_ASSERT_DOUBLE_EQ( 7.0, m2[0][2])
        CER_ASSERT_DOUBLE_EQ( 8.0, m2[1][2])
        CER_ASSERT_DOUBLE_EQ( 9.0, m2[2][2])
        CER_ASSERT_DOUBLE_EQ(10.0, m2[0][3])
        CER_ASSERT_DOUBLE_EQ(11.0, m2[1][3])
        CER_ASSERT_DOUBLE_EQ(12.0, m2[2][3])
    }

    TEST(mat_test, componentwise_constructor_with_matching_type) {
        constexpr auto m1 = mat3x3d(
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        );
        CER_ASSERT_DOUBLE_EQ(1.0, m1[0][0])
        CER_ASSERT_DOUBLE_EQ(2.0, m1[1][0])
        CER_ASSERT_DOUBLE_EQ(3.0, m1[2][0])
        CER_ASSERT_DOUBLE_EQ(4.0, m1[0][1])
        CER_ASSERT_DOUBLE_EQ(5.0, m1[1][1])
        CER_ASSERT_DOUBLE_EQ(6.0, m1[2][1])
        CER_ASSERT_DOUBLE_EQ(7.0, m1[0][2])
        CER_ASSERT_DOUBLE_EQ(8.0, m1[1][2])
        CER_ASSERT_DOUBLE_EQ(9.0, m1[2][2])

        constexpr auto m2 = mat<double, 4, 3>(
             1.0,  2.0,  3.0,
             4.0,  5.0,  6.0,
             7.0,  8.0,  9.0,
            10.0, 11.0, 12.0
        );
        CER_ASSERT_DOUBLE_EQ( 1.0, m2[0][0])
        CER_ASSERT_DOUBLE_EQ( 2.0, m2[1][0])
        CER_ASSERT_DOUBLE_EQ( 3.0, m2[2][0])
        CER_ASSERT_DOUBLE_EQ( 4.0, m2[0][1])
        CER_ASSERT_DOUBLE_EQ( 5.0, m2[1][1])
        CER_ASSERT_DOUBLE_EQ( 6.0, m2[2][1])
        CER_ASSERT_DOUBLE_EQ( 7.0, m2[0][2])
        CER_ASSERT_DOUBLE_EQ( 8.0, m2[1][2])
        CER_ASSERT_DOUBLE_EQ( 9.0, m2[2][2])
        CER_ASSERT_DOUBLE_EQ(10.0, m2[0][3])
        CER_ASSERT_DOUBLE_EQ(11.0, m2[1][3])
        CER_ASSERT_DOUBLE_EQ(12.0, m2[2][3])
    }

    TEST(mat_test, componentwise_constructor_with_mixed_types) {
        constexpr auto m1 = mat3x3d(
            1.0f, 2.0, 3,
            4.0f, 5.0, 6,
            7.0f, 8.0, 9
        );
        CER_ASSERT_DOUBLE_EQ(1.0, m1[0][0])
        CER_ASSERT_DOUBLE_EQ(2.0, m1[1][0])
        CER_ASSERT_DOUBLE_EQ(3.0, m1[2][0])
        CER_ASSERT_DOUBLE_EQ(4.0, m1[0][1])
        CER_ASSERT_DOUBLE_EQ(5.0, m1[1][1])
        CER_ASSERT_DOUBLE_EQ(6.0, m1[2][1])
        CER_ASSERT_DOUBLE_EQ(7.0, m1[0][2])
        CER_ASSERT_DOUBLE_EQ(8.0, m1[1][2])
        CER_ASSERT_DOUBLE_EQ(9.0, m1[2][2])

        constexpr auto m2 = mat<double, 4, 3>(
             1.0f,  2.0,  3,
             4.0f,  5.0,  6,
             7.0f,  8.0,  9,
            10.0f, 11.0, 12
        );
        CER_ASSERT_DOUBLE_EQ( 1.0, m2[0][0])
        CER_ASSERT_DOUBLE_EQ( 2.0, m2[1][0])
        CER_ASSERT_DOUBLE_EQ( 3.0, m2[2][0])
        CER_ASSERT_DOUBLE_EQ( 4.0, m2[0][1])
        CER_ASSERT_DOUBLE_EQ( 5.0, m2[1][1])
        CER_ASSERT_DOUBLE_EQ( 6.0, m2[2][1])
        CER_ASSERT_DOUBLE_EQ( 7.0, m2[0][2])
        CER_ASSERT_DOUBLE_EQ( 8.0, m2[1][2])
        CER_ASSERT_DOUBLE_EQ( 9.0, m2[2][2])
        CER_ASSERT_DOUBLE_EQ(10.0, m2[0][3])
        CER_ASSERT_DOUBLE_EQ(11.0, m2[1][3])
        CER_ASSERT_DOUBLE_EQ(12.0, m2[2][3])
    }

    TEST(mat_test, converting_constructor) {
        constexpr auto from = mat4x4d(
            1, 2, 3, 4,
            5, 6, 7, 8,
            7, 6, 5, 4,
            3, 2, 1, 0);
        constexpr auto to = mat4x4f(
            1, 2, 3, 4,
            5, 6, 7, 8,
            7, 6, 5, 4,
            3, 2, 1, 0);
        CER_ASSERT_EQ(to, mat4x4f(from));
    }

        TEST(mat_test, fill) {
        CER_ASSERT_EQ(
            mat4x4f(
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1),
            mat4x4f::fill(1.0f))
        CER_ASSERT_EQ(
            mat4x4f(
                -2, -2, -2, -2,
                -2, -2, -2, -2,
                -2, -2, -2, -2,
                -2, -2, -2, -2),
            mat4x4f::fill(-2.0f))
    }

    TEST(mat_test, identity) {
        CER_ASSERT_EQ(
            mat4x4f(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1),
            mat4x4f::identity())
    }

    TEST(mat_test, zero) {
        CER_ASSERT_EQ(
            mat4x4f(
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0),
            mat4x4f::zero())
    }

    TEST(mat_test, rotate_x_90_cw) {
        constexpr auto m = mat4x4d::rot_90_x_cw();
        constexpr auto v = vec4d::pos_y();
        CER_ASSERT_VEC_EQ(vec4d::neg_z(), m * v)
    }

    TEST(mat_test, rotate_y_90_cw) {
        constexpr auto m = mat4x4d::rot_90_y_cw();
        constexpr auto v = vec4d::pos_x();
        CER_ASSERT_VEC_EQ(vec4d::pos_z(), m * v)
    }

    TEST(mat_test, rotate_z_90_cw) {
        constexpr auto m = mat4x4d::rot_90_z_cw();
        constexpr auto v = vec4d::pos_y();
        CER_ASSERT_VEC_EQ(vec4d::pos_x(), m * v)
    }

    TEST(mat_test, rotate_x_90_ccw) {
        constexpr auto m = mat4x4d::rot_90_x_ccw();
        constexpr auto v = vec4d::pos_y();
        CER_ASSERT_VEC_EQ(vec4d::pos_z(), m * v)
    }

    TEST(mat_test, rotate_y_90_ccw) {
        constexpr auto m = mat4x4d::rot_90_y_ccw();
        constexpr auto v = vec4d::pos_x();
        CER_ASSERT_VEC_EQ(vec4d::neg_z(), m * v)
    }

    TEST(mat_test, rotate_z_90_ccw) {
        constexpr auto m = mat4x4d::rot_90_z_ccw();
        constexpr auto v = vec4d::pos_x();
        CER_ASSERT_VEC_EQ(vec4d::pos_y(), m * v)
    }

    TEST(mat_test, rotate_x_180) {
        constexpr auto m = mat4x4d::rot_180_x();
        constexpr auto v = vec4d::pos_y();
        CER_ASSERT_VEC_EQ(vec4d::neg_y(), m * v)
    }

    TEST(mat_test, rotate_y_180) {
        constexpr auto m = mat4x4d::rot_180_y();
        constexpr auto v = vec4d::pos_x();
        CER_ASSERT_VEC_EQ(vec4d::neg_x(), m * v)
    }

    TEST(mat_test, rotate_z_180) {
        constexpr auto m = mat4x4d::rot_180_z();
        constexpr auto v = vec4d::pos_y();
        CER_ASSERT_VEC_EQ(vec4d::neg_y(), m * v)
    }

    TEST(mat_test, mirror_x) {
        constexpr auto m = mat4x4d::mirror_x();
        constexpr auto v = vec4d(1.0, 1.0, 1.0, 0.0);
        CER_ASSERT_VEC_EQ(vec4d(-1.0, 1.0, 1.0, 0.0), m * v)
    }

    TEST(mat_test, mirror_y) {
        constexpr auto m = mat4x4d::mirror_y();
        constexpr auto v = vec4d(1.0, 1.0, 1.0, 0.0);
        CER_ASSERT_VEC_EQ(vec4d(1.0, -1.0, 1.0, 0.0), m * v)
    }

    TEST(mat_test, mirror_z) {
        constexpr auto m = mat4x4d::mirror_z();
        constexpr auto v = vec4d(1.0, 1.0, 1.0, 0.0);
        CER_ASSERT_VEC_EQ(vec4d(1.0, 1.0, -1.0, 0.0), m * v)
    }

    TEST(mat_test, zero_out_x) {
        constexpr auto m = mat4x4d::zero_out<0>();
        constexpr vec4d v(1.0, 1.0, 1.0, 1.0);
        CER_ASSERT_VEC_EQ(vec4d(0.0, 1.0, 1.0, 1.0), m * v)
    }

    TEST(mat_test, zero_out_y) {
        constexpr auto m = mat4x4d::zero_out<1>();
        constexpr vec4d v(1.0, 1.0, 1.0, 1.0);
        CER_ASSERT_VEC_EQ(vec4d(1.0, 0.0, 1.0, 1.0), m * v)
    }

    TEST(mat_test, zero_out_z) {
        constexpr auto m = mat4x4d::zero_out<2>();
        constexpr vec4d v(1.0, 1.0, 1.0, 1.0);
        CER_ASSERT_VEC_EQ(vec4d(1.0, 1.0, 0.0, 1.0), m * v)
    }

    TEST(mat_test, operator_subscript) {
        constexpr auto m =  mat4x4d(
            1, 2, 3, 4,
            5, 6, 7, 8,
            7, 6, 5, 4,
            3, 2, 1, 0);

        CER_ASSERT_DOUBLE_EQ(m.v[0][0], m[0][0])
        CER_ASSERT_DOUBLE_EQ(m.v[0][1], m[0][1])
        CER_ASSERT_DOUBLE_EQ(m.v[0][2], m[0][2])
        CER_ASSERT_DOUBLE_EQ(m.v[0][3], m[0][3])

        CER_ASSERT_DOUBLE_EQ(m.v[1][0], m[1][0])
        CER_ASSERT_DOUBLE_EQ(m.v[1][1], m[1][1])
        CER_ASSERT_DOUBLE_EQ(m.v[1][2], m[1][2])
        CER_ASSERT_DOUBLE_EQ(m.v[1][3], m[1][3])

        CER_ASSERT_DOUBLE_EQ(m.v[2][0], m[2][0])
        CER_ASSERT_DOUBLE_EQ(m.v[2][1], m[2][1])
        CER_ASSERT_DOUBLE_EQ(m.v[2][2], m[2][2])
        CER_ASSERT_DOUBLE_EQ(m.v[2][3], m[2][3])

        CER_ASSERT_DOUBLE_EQ(m.v[3][0], m[3][0])
        CER_ASSERT_DOUBLE_EQ(m.v[3][1], m[3][1])
        CER_ASSERT_DOUBLE_EQ(m.v[3][2], m[3][2])
        CER_ASSERT_DOUBLE_EQ(m.v[3][3], m[3][3])
    }

    TEST(mat_test, compare) {
        CER_ASSERT_TRUE(
            compare(
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4)
            ) == 0
        )

        CER_ASSERT_TRUE(
            compare(
                mat4x4d(
                    1, 2, 3, 1,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4)
            ) < 0
        )

        CER_ASSERT_TRUE(
            compare(
                mat4x4d(
                    1, 2, 3, 5,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4)
            ) > 0
        )
    }

    TEST(mat_test, is_equal) {
        CER_ASSERT_TRUE(
            is_equal(
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                0.0
            )
        )
        CER_ASSERT_TRUE(
            is_equal(
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                mat4x4d(
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4,
                    1, 2, 3, 4),
                0.1
            )
        )
        CER_ASSERT_TRUE(
            is_equal(
                mat4x4d(
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4),
                mat4x4d(
                    1.1, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0),
                0.11
            )
        )
        CER_ASSERT_TRUE(
            is_equal(
                mat4x4d(
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4),
                mat4x4d(
                    1.1, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0),
                0.1
            )
        )
        CER_ASSERT_FALSE(
            is_equal(
                mat4x4d(
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4),
                mat4x4d(
                    1.11, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0,
                    1.0, 2.0, 3.0, 4.0),
                0.1
            )
        )
    }

    TEST(mat_test, is_zero) {
        ASSERT_TRUE(is_zero(mat4x4d::zero(), vm::Cd::almostZero()));
        ASSERT_FALSE(is_zero(mat4x4d::identity(), vm::Cd::almostZero()));
    }

    TEST(mat_test, operator_equal) {
        constexpr auto m = mat4x4d(
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16);
        constexpr auto n = mat4x4d(
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16);
        constexpr auto o = mat4x4d(
             2,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16);
        CER_ASSERT_TRUE(m == n)
        CER_ASSERT_FALSE(m == o)
    }

    TEST(mat_test, operator_not_equal) {
        constexpr auto m = mat4x4d(
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16);
        constexpr auto n = mat4x4d(
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16);
        constexpr auto o = mat4x4d(
             2,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16);
        CER_ASSERT_FALSE(m != n)
        CER_ASSERT_TRUE(m != o)
    }

    TEST(mat_test, operator_unary_plus) {
        CER_ASSERT_EQ(
            mat4x4d(
                +1, +2, +3, +4,
                +1, +2, +3, +4,
                +1, +2, +3, +4,
                +1, +2, +3, +4),
            +mat4x4d(
                +1, +2, +3, +4,
                +1, +2, +3, +4,
                +1, +2, +3, +4,
                +1, +2, +3, +4)
        )
    }

    TEST(mat_test, operator_unary_minus) {
        CER_ASSERT_EQ(
            mat4x4d(
                -1, -2, -3, -4,
                -1, -2, -3, -4,
                -1, -2, -3, -4,
                -1, -2, -3, -4),
            -mat4x4d(
                +1, +2, +3, +4,
                +1, +2, +3, +4,
                +1, +2, +3, +4,
                +1, +2, +3, +4)
        )
    }

    TEST(mat_test, operator_binary_plus) {
        CER_ASSERT_EQ(
            mat4x4d(
                 3,  4,  6,  8,
                10, 14, 14, 16,
                18, 21, 22, 24,
                26, 28, 30, 32
                ),
              mat4x4d(
                 1,  2,  3,  4,
                 5,  6,  7,  8,
                 9, 10, 11, 12,
                13, 14, 15, 16)
            + mat4x4d(
                 2,  2,  3,  4,
                 5,  8,  7,  8,
                 9, 11, 11, 12,
                13, 14, 15, 16))
    }

    TEST(mat_test, operator_binary_minus) {
        CER_ASSERT_EQ(
            mat4x4d(
                -1,  0,  0,  0,
                 0, -2,  0,  0,
                 0, -1,  0,  0,
                 0,  0,  0,  0
                ),
              mat4x4d(
                 1,  2,  3,  4,
                 5,  6,  7,  8,
                 9, 10, 11, 12,
                13, 14, 15, 16)
            - mat4x4d(
                 2,  2,  3,  4,
                 5,  8,  7,  8,
                 9, 11, 11, 12,
                13, 14, 15, 16))
    }

    TEST(mat_test, operator_multiply_matrix) {
        CER_ASSERT_EQ(
            mat4x4d(
                 91, 107, 110, 120,
                207, 247, 254, 280,
                323, 387, 398, 440,
                439, 527, 542, 600
                ),
              mat4x4d(
                 1,  2,  3,  4,
                 5,  6,  7,  8,
                 9, 10, 11, 12,
                13, 14, 15, 16)
            * mat4x4d(
                 2,  2,  3,  4,
                 5,  8,  7,  8,
                 9, 11, 11, 12,
                13, 14, 15, 16))

        constexpr auto exp = mat<double, 4, 2>(
             39,  51,
            103, 135,
            167, 219,
            231, 303
        );
        constexpr auto lhs = mat<double, 4, 3>(
            1,  2,  3,
            5,  6,  7,
            9, 10, 11,
            13, 14, 15);
        constexpr auto rhs = mat<double, 3, 2>(
            2,  2,
            5,  8,
            9, 11);

        CER_ASSERT_EQ(exp, lhs * rhs)
    }



    /*

    TEST(mat_test, multiply) {
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const mat4x4d n( 2.0,  2.0,  3.0,  4.0,
                         5.0,  8.0,  7.0,  8.0,
                         9.0, 11.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const mat4x4d r( 91.0, 107.0, 110.0, 120.0,
                        207.0, 247.0, 254.0, 280.0,
                        323.0, 387.0, 398.0, 440.0,
                        439.0, 527.0, 542.0, 600.0);
        const mat4x4d o = m * n;
        ASSERT_MAT_EQ(r, o);
    }

    TEST(mat_test, scalarMultiply_right) {
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const mat4x4d o = m * 3.0;

        for (size_t c = 0; c < 4; ++c) {
            for (size_t r = 0; r < 4; ++r) {
                ASSERT_DOUBLE_EQ(m[c][r] * 3.0, o[c][r]);
            }
        }
    }

    TEST(mat_test, scalarMultiply_left) {
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                         13.0, 14.0, 15.0, 16.0);
        const mat4x4d o = 3.0 * m;

        for (size_t c = 0; c < 4; ++c) {
            for (size_t r = 0; r < 4; ++r) {
                ASSERT_DOUBLE_EQ(m[c][r] * 3.0, o[c][r]);
            }
        }
    }

    TEST(mat_test, divide) {
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const mat4x4d o = m / 3.0;

        for (size_t c = 0; c < 4; ++c) {
            for (size_t r = 0; r < 4; ++r) {
                ASSERT_DOUBLE_EQ(m[c][r] / 3.0, o[c][r]);
            }
        }
    }

    TEST(mat_test, vectorMultiply_right) {
        const vec4d v(1.0, 2.0, 3.0, 1.0);
        ASSERT_VEC_EQ(v, mat4x4d::identity * v);

        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                         13.0, 14.0, 15.0, 16.0);
        const vec4d r(18.0, 46.0, 74.0, 102.0);
        ASSERT_VEC_EQ(r, m * v)
    }

    TEST(mat_test, vectorMultiply_left) {
        const vec4d v(1.0, 2.0, 3.0, 1.0);
        ASSERT_VEC_EQ(v, v * mat4x4d::identity);

        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                         13.0, 14.0, 15.0, 16.0);
        const vec4d r(51.0, 58.0, 65.0, 72.0);
        ASSERT_VEC_EQ(r, v * m);
    }

    TEST(mat_test, vectorMultiply_right_2) {
        const vec3d v(1.0, 2.0, 3.0);
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const vec4d r(18.0, 46.0, 74.0, 102.0);
        ASSERT_VEC_EQ(to_cartesian_coords(r), m * v)
    }

    TEST(mat_test, vectorMultiply_left_2) {
        const vec3d v(1.0, 2.0, 3.0);
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const vec4d r(51.0, 58.0, 65.0, 72.0);
        ASSERT_VEC_EQ(to_cartesian_coords(r), v * m);
    }


    TEST(mat_test, transpose) {
        mat<double, 4, 4> m;
        for (size_t c = 0; c < 4; ++c) {
            for (size_t r = 0; r < 4; ++r) {
                m[c][r] = static_cast<double>(c * 4 + r);
            }
        }

        constexpr auto t = transpose(m);

        for (size_t c = 0; c < 4; ++c) {
            for (size_t r = 0; r < 4; ++r) {
                ASSERT_DOUBLE_EQ(m[c][r], t[r][c]);
            }
        }
    }

    TEST(mat_test, extractMinor) {
        const mat4x4d m( 1.0,  2.0,  3.0,  4.0,
                         5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0,
                        13.0, 14.0, 15.0, 16.0);
        const mat3x3d m00( 6.0,  7.0,  8.0,
                          10.0, 11.0, 12.0,
                          14.0, 15.0, 16.0);
        const mat3x3d m33( 1.0,  2.0,  3.0,
                           5.0,  6.0,  7.0,
                           9.0, 10.0, 11.0);
        const mat3x3d m12( 1.0,  2.0,  4.0,
                           9.0, 10.0, 12.0,
                          13.0, 14.0, 16.0);
        const mat3x3d m21( 1.0,  3.0,  4.0,
                           5.0,  7.0,  8.0,
                          13.0, 15.0, 16.0);
        ASSERT_MAT_EQ(m00, extractMinor(m, 0, 0));
        ASSERT_MAT_EQ(m33, extractMinor(m, 3, 3));
        ASSERT_MAT_EQ(m12, extractMinor(m, 1, 2));
        ASSERT_MAT_EQ(m21, extractMinor(m, 2, 1));
    }

    TEST(mat_test, determinant) {
        const mat4x4d m1( 1.0,  2.0,  3.0,  4.0,
                          5.0,  6.0,  7.0,  8.0,
                          9.0, 10.0, 11.0, 12.0,
                         13.0, 14.0, 15.0, 16.0);
        const mat4x4d m2(65.0, 12.0, -3.0, -5.0,
                         -5.0,  1.0,  0.0,  0.0,
                         19.0, 10.0, 11.0,  8.0,
                          0.0,  1.0, -8.0,  3.0);
        const mat4x4d m3( 3.0,  2.0, -1.0,  4.0,
                          2.0,  1.0,  5.0,  7.0,
                          0.0,  5.0,  2.0, -6.0,
                         -1.0,  2.0,  1.0,  0.0);
        ASSERT_DOUBLE_EQ(0.0, computeDeterminant(mat4x4d::zero));
        ASSERT_DOUBLE_EQ(1.0, computeDeterminant(mat4x4d::identity));
        ASSERT_DOUBLE_EQ(0.0, computeDeterminant(m1));
        ASSERT_DOUBLE_EQ(15661.0, computeDeterminant(m2));
        ASSERT_DOUBLE_EQ(-418.0, computeDeterminant(m3));
    }

    TEST(mat_test, computeAdjugate) {
        const mat4x4d m1( 1.0,  2.0,  3.0,  4.0,
                          5.0,  6.0,  7.0,  8.0,
                          9.0, 10.0, 11.0, 12.0,
                         13.0, 14.0, 15.0, 16.0);
        const mat4x4d m2(65.0, 12.0, -3.0, -5.0,
                         -5.0,  1.0,  0.0,  0.0,
                         19.0, 10.0, 11.0,  8.0,
                          0.0,  1.0, -8.0,  3.0);
        const mat4x4d m3( 3.0,  2.0, -1.0,  4.0,
                          2.0,  1.0,  5.0,  7.0,
                          0.0,  5.0,  2.0, -6.0,
                         -1.0,  2.0,  1.0,  0.0);
        const mat4x4d r1(0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0);
        const mat4x4d r2(  97.0, -1685.0,  49.0,    31.0,
                          485.0,  7236.0, 245.0,	   155.0,
                         -167.0,	  -651.0, 400.0, -1345.0,
                         -607.0, -4148.0, 985.0,  1582.0);
        const mat4x4d r3(-47.0, -28.0, -64.0,  221.0,
                         -56.0,  20.0, -14.0, -128.0,
                          65.0, -68.0, -36.0,   59.0,
                         -25.0,  -6.0,  46.0,  -87.0);

        ASSERT_MAT_EQ(mat4x4d::identity, computeAdjugate(mat4x4d::identity));
        ASSERT_MAT_EQ(mat4x4d::zero, computeAdjugate(mat4x4d::zero));
        ASSERT_MAT_EQ(r1, computeAdjugate(m1));
        ASSERT_MAT_EQ(r2, computeAdjugate(m2));
        ASSERT_MAT_EQ(r3, computeAdjugate(m3));
    }

    template <typename T, size_t S>
    void ASSERT_INVERTIBLE(const mat<T,S,S>& expected, const mat<T,S,S>& actual) {
        auto [invertible, inverse] = invert(actual);
        ASSERT_MAT_EQ(expected, inverse);
        ASSERT_TRUE(invertible);
    }

    template <typename T, size_t S>
    void ASSERT_NOT_INVERTIBLE(const mat<T,S,S>& actual) {
        bool invertible = false;
        mat<T,S,S> inverse;
        std::tie(invertible, inverse) = invert(actual);
        ASSERT_FALSE(invertible);
    }

    TEST(mat_test, invert) {
        const mat4x4d m1( 1.0,  2.0,  3.0,  4.0,
                          5.0,  6.0,  7.0,  8.0,
                          9.0, 10.0, 11.0, 12.0,
                         13.0, 14.0, 15.0, 16.0);
        const mat4x4d m2(65.0, 12.0, -3.0, -5.0,
                         -5.0,  1.0,  0.0,  0.0,
                         19.0, 10.0, 11.0,  8.0,
                          0.0,  1.0, -8.0,  3.0);
        const mat4x4d m3( 0.0, -1.0,  0.0,    0.0,
                          0.0,  0.0,  1.0,  128.0,
                         -1.0,  0.0,  0.0,    0.0,
                          0.0,  0.0,  0.0,    1.0);
        const mat4x4d m4( 0.0,  0.0, -1.0,    0.0,
                         -1.0,  0.0,  0.0,    0.0,
                          0.0,  1.0,  0.0, -128.0,
                          0.0,  0.0,  0.0,    1.0);
        const mat4x4d r2( 0.0061937296468936, -0.10759210778367, 0.0031287912649256, 0.0019794393716876,
                          0.030968648234468,   0.46203946108167, 0.015643956324628,  0.0098971968584382,
                         -0.01066343145393,   -0.04156822680544, 0.025541153183066, -0.08588212757806,
                         -0.038758699955303,  -0.2648617585084,  0.062895089713301,  0.10101526083903);

        ASSERT_INVERTIBLE(mat4x4d::identity, mat4x4d::identity);
        ASSERT_INVERTIBLE(r2, m2);
        ASSERT_INVERTIBLE(m4, m3);
        ASSERT_NOT_INVERTIBLE(mat4x4d::zero);
        ASSERT_NOT_INVERTIBLE(m1);
    }

    TEST(mat_test, stripTranslation) {
        const vec3d v(2.0, 3.0, 4.0);
        const mat4x4d t = translationMatrix(v);
        const mat4x4d r = rotationMatrix(toRadians(15.0), toRadians(30.0), toRadians(45.0));
        ASSERT_EQ(r, stripTranslation(r * t));
        ASSERT_EQ(r, stripTranslation(t * r));
    }

    TEST(mat_test, lupSolve) {
        constexpr auto A = rotationMatrix(0.1, 0.2, 0.3) * translationMatrix(vec3d(100.0, 100.0, 100.0));
        constexpr auto x = vec4d(20, -60, 32, 1);
        constexpr auto b = A * x;

        // solve for x
        auto [success, x2] = lupSolve(A, b);
        EXPECT_TRUE(success);
        EXPECT_VEC_EQ(x, x2);
        EXPECT_VEC_EQ(b, A * x2);
    }

    TEST(mat_test, pointsTransformationMatrix) {
        const vec3d in[3] = {{2.0, 0.0, 0.0},
                             {4.0, 0.0, 0.0},
                             {2.0, 2.0, 0.0}};

        constexpr auto M = translationMatrix(vec3d(100.0, 100.0, 100.0)) * scalingMatrix(vec3d(2.0, 2.0, 2.0)) * rotationMatrix(vec3d::pos_z(), toRadians(90.0));

        vec3d out[3];
        for (size_t i=0; i<3; ++i) {
            out[i] = M * in[i];
        }

        // in[0]: 0,2,0, then 0,4,0, then 100, 104, 100
        // in[1]: 0,4,0, then 0,8,0, then 100, 108, 100
        // in[2]: -2,2,0, then -4,4,0, then 96, 104, 100

        constexpr auto M2 = pointsTransformationMatrix(in[0], in[1], in[2], out[0], out[1], out[2]);
        vec3d test[3];
        for (size_t i=0; i<3; ++i) {
            test[i] = M2 * in[i];

            EXPECT_VEC_EQ(out[i], test[i]);
        }
    }
     */
}
