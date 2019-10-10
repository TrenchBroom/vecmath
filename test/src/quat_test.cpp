/*
 Copyright 2010-2019 Kristian Duske
 Copyright 2015-2019 Eric Wasylishen

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <gtest/gtest.h>

#include <vecmath/forward.h>
#include <vecmath/quat.h>
#include <vecmath/vec.h>
#include <vecmath/scalar.h>

#include "test_utils.h"

namespace vm {
    TEST(quat_test, constructor_default) {
        constexpr auto q = quatf();
        CER_ASSERT_FLOAT_EQ(0.0f, q.r)
        CER_ASSERT_TRUE(is_zero(q.v, vm::Cf::almost_zero()))
    }

    TEST(quat_test, construtor_with_rotation) {
        const auto angle = to_radians(15.0f);
        const auto axis = normalize(vec3f(1, 2, 3));
        const auto q = quatf(axis, angle);

        ASSERT_FLOAT_EQ(std::cos(angle / 2.0f), q.r);
        ASSERT_VEC_EQ(axis * std::sin(angle / 2.0f), q.v);
    }

    TEST(quat_test, constructor_with_vector_rotation) {
        const auto from = vec3f(0, 1, 0);
        const auto to = vec3f(1, 0, 0);
        const auto q = quatf(from, to);
        ASSERT_VEC_EQ(to, q * from);
    }

    TEST(quat_test, constructor_with_opposite_vector_rotation) {
        for (std::size_t i = 0; i < 3; ++i) {
            auto from = vec3d(0, 0, 0);
            auto to   = vec3d(0, 0, 0);

            from[i] = 1.0;
            to[i] = -1.0;

            const auto q = quatd(from, to);
            EXPECT_VEC_EQ(to, q * from);

            // The quaternion axis should be perpendicular to both from and to vectors
            EXPECT_DOUBLE_EQ(0.0, dot(q.axis(), from));
            EXPECT_DOUBLE_EQ(0.0, dot(q.axis(), to));
        }
    }

    TEST(quat_test, constructor_with_equal_vector_rotation) {
        for (std::size_t i = 0; i < 3; ++i) {
            auto from = vec3d(0, 0, 0);
            from[i] = 1.0;

            const auto to = from;
            const auto q = quatd(from, to);
            EXPECT_VEC_EQ(to, q * from);
        }
    }

    TEST(quat_test, angle) {
        const auto angle = to_radians(15.0f);
        const auto q = quatf(vec3f::pos_z(), angle);

        ASSERT_NEAR(angle, q.angle(), 0.001f);
    }

    TEST(quat_test, axis) {
        ASSERT_VEC_EQ(vec3d::zero(), quatd().axis());
        ASSERT_VEC_EQ(vec3d::pos_z(), quatd(vec3d::pos_z(), to_radians(45.0)).axis());
        ASSERT_VEC_EQ(normalize(vec3d(1, 1, 0)), quatd(normalize(vec3d(1, 1, 0)), to_radians(25.0)).axis());
    }

    TEST(quat_test, conjugate) {
        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0, 0, 0.1305262f));
        constexpr auto p = q.conjugate();

        CER_ASSERT_VEC_EQ(-q.v, p.v)
    }

    TEST(quat_test, is_equal) {
        CER_ASSERT_TRUE(is_equal(quatd(), quatd(), 0.0))

        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0, 0, 0.1305262f));
        CER_ASSERT_TRUE(is_equal(q,  q, 0.0f))
        CER_ASSERT_TRUE(is_equal(q, -q, 0.0f))
    }

    TEST(quat_test, operator_equal) {
        CER_ASSERT_TRUE(quatd() == quatd())

        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0, 0, 0.1305262f));
        constexpr auto p = quatf(0.991444885f, vec3f(0.1305262f, 0, 0));

        CER_ASSERT_TRUE(q == q)
        CER_ASSERT_TRUE(q == -q)
        CER_ASSERT_TRUE(p == p)
        CER_ASSERT_TRUE(p == -p)
        CER_ASSERT_FALSE(q == p)
    }

    TEST(quat_test, operator_not_equal) {
        CER_ASSERT_FALSE(quatd() != quatd())

        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0, 0, 0.1305262f));
        constexpr auto p = quatf(0.991444885f, vec3f(0.1305262f, 0, 0));

        CER_ASSERT_FALSE(q != q)
        CER_ASSERT_FALSE(q != -q)
        CER_ASSERT_FALSE(p != p)
        CER_ASSERT_FALSE(p != -p)
        CER_ASSERT_TRUE(q != p)
    }

    TEST(quat_test, operator_unary_plus) {
        CER_ASSERT_EQ(quatf(), +quatf())
    }

    TEST(quat_test, operator_unary_minus) {
        // create quaternion with axis pos_x and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0.1305262f, 0, 0));
        constexpr auto nq = -q;

        CER_ASSERT_FLOAT_EQ(-(q.r), nq.r)
        CER_ASSERT_VEC_EQ(q.v, nq.v)
    }

    TEST(quat_test, operator_multiply_scalar_right) {
        // create quaternion with axis pos_x and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0.1305262f, 0, 0));
        constexpr auto p = q * 2.0f;
        CER_ASSERT_FLOAT_EQ(q.r * 2.0f, p.r)
    }

    TEST(quat_test, operator_multiply_scalar_left) {
        // create quaternion with axis pos_x and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0.1305262f, 0, 0));
        constexpr auto p = 2.0f * q;
        CER_ASSERT_FLOAT_EQ(q.r * 2.0f, p.r)
    }

    TEST(quat_test, operator_multiply_quaternions) {
        // constexpr auto angle1 = to_radians(15.0f);
        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q1 = quatf(0.991444885f, vec3f(0, 0, 0.1305262f));

        // const float angle2 = to_radians(10.0f);
        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q2 = quatf(0.99619472f, vec3f(0, 0, 0.0871557369f));
        constexpr quatf q = q1 * q2;

        constexpr auto v = vec3f::pos_x();
        constexpr auto w = q * v;

        constexpr auto cos_a1_a2 = 0.906307756f; // std::cos(angle1 + angle2)
        constexpr auto sin_a1_a2 = 0.42261827f; // std::sin(angle1 + angle2)

        CER_ASSERT_VEC_EQ(vec3f(cos_a1_a2, sin_a1_a2, 0.0f), w)
    }

    TEST(quat_test, operator_multiply_vector) {
        // constexpr auto angle = to_radians(15.0f);
        // create quaternion with axis pos_z and angle 15.0f to_degrees
        constexpr auto q = quatf(0.991444885f, vec3f(0, 0, 0.1305262f));
        constexpr auto x = vec3f::pos_x();

        constexpr auto cos_a = 0.965925812f; // std::cos(angle);
        constexpr auto sin_a = 0.258819044f; // std::sin(angle);

        CER_ASSERT_VEC_EQ(vec3f(cos_a, sin_a, 0), q * x)
    }
}
