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
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/ray.h>
#include <vecmath/ray_io.h>
#include <vecmath/scalar.h>
#include <vecmath/util.h>

#include "test_utils.h"

namespace vm {
    TEST(ray_test, constructor_default) {
        constexpr auto r = ray3d();
        CER_ASSERT_VEC_EQ(vec3d::zero(), r.origin)
        CER_ASSERT_VEC_EQ(vec3d::zero(), r.direction)
    }

    TEST(ray_test, constructor_convert) {
        constexpr auto r = ray3d(vec3d::one(), vec3d::pos_z());
        constexpr auto s = ray3f(r);
        CER_ASSERT_VEC_EQ(vec3f::one(), s.origin)
        CER_ASSERT_VEC_EQ(vec3f::pos_z(), s.direction)
    }

    TEST(ray_test, constructor_with_origin_and_direction) {
        constexpr auto r = ray3d(vec3d::one(), vec3d::pos_z());
        CER_ASSERT_VEC_EQ(vec3d::one(), r.origin)
        CER_ASSERT_VEC_EQ(vec3d::pos_z(), r.direction)
    }

    TEST(ray_test, get_origin) {
        const auto r = ray3d(vec3d::one(), vec3d::pos_z());
        ASSERT_VEC_EQ(r.origin, r.get_origin());
    }

    TEST(ray_test, get_direction) {
        const auto r = ray3d(vec3d::one(), vec3d::pos_z());
        ASSERT_VEC_EQ(r.direction, r.get_direction());
    }

    TEST(ray_test, transform) {
        const auto r = ray3d(vec3d::one(), vec3d::pos_z());
        const auto rm = rotation_matrix(to_radians(15.0), to_radians(20.0), to_radians(-12.0));
        const auto tm = translation_matrix(vec3d::one());

        const auto rt = r.transform(rm * tm);
        ASSERT_TRUE(is_unit(r.direction, vm::Cd::almost_zero()));
        ASSERT_VEC_EQ(rm * tm * r.origin, rt.origin);
        ASSERT_VEC_EQ(rm * r.direction, rt.direction);
    }

    TEST(ray_test, transform_c) {
        constexpr auto r = ray3d(vec3d::one(), vec3d::pos_z());
        constexpr auto sm = scaling_matrix(vec3d(2.0, 0.5, -2.0));
        constexpr auto tm = translation_matrix(vec3d::one());

        constexpr auto rt = r.transform_c(sm * tm);
        CER_ASSERT_TRUE(is_unit_c(r.direction, vm::Cd::almost_zero()))
        CER_ASSERT_VEC_EQ(sm * tm * r.origin, rt.origin)
        CER_ASSERT_VEC_EQ(normalize_c(sm * r.direction), rt.direction)
    }

    TEST(ray_test, point_status) {
        constexpr auto ray =  ray3f(vec3f::zero(), vec3f::pos_z());
        CER_ASSERT_EQ(plane_status::above, ray.point_status(vec3f(0.0f, 0.0f, 1.0f)))
        CER_ASSERT_EQ(plane_status::inside, ray.point_status(vec3f(0.0f, 0.0f, 0.0f)))
        CER_ASSERT_EQ(plane_status::below, ray.point_status(vec3f(0.0f, 0.0f, -1.0f)))
    }

    TEST(ray_test, point_at_distance) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_x());
        CER_ASSERT_VEC_EQ(vec3f(5.0f, 0.0f, 0.0f), point_at_distance(ray, 5.0f))
    }

    TEST(ray_test, is_equal) {
        CER_ASSERT_TRUE(is_equal(ray3d(), ray3d(), 0.0))
        CER_ASSERT_TRUE(is_equal(ray3d(vec3d::zero(), vec3d::pos_z()), ray3d(vec3d::zero(), vec3d::pos_z()), 0.0))
        CER_ASSERT_FALSE(is_equal(ray3d(vec3d(0, 0, 0), vec3d(0, 0, 1)), ray3d(vec3d(1, 0, 0), vec3d(0, 0, 1)), 0.0))
        CER_ASSERT_TRUE(is_equal(ray3d(vec3d(0, 0, 0), vec3d(0, 0, 1)), ray3d(vec3d(1, 0, 0), vec3d(0, 0, 1)), 2.0))
    }

    TEST(ray_test, operator_equal) {
        CER_ASSERT_TRUE(ray3d() == ray3d())
        CER_ASSERT_TRUE(ray3d(vec3d::zero(), vec3d::pos_z()) == ray3d(vec3d::zero(), vec3d::pos_z()))
        CER_ASSERT_FALSE(ray3d(vec3d(0, 0, 0), vec3d(0, 0, 1)) == ray3d(vec3d(1, 0, 0), vec3d(0, 0, 1)))
    }

    TEST(ray_test, operator_not_equal) {
        CER_ASSERT_FALSE(ray3d() != ray3d())
        CER_ASSERT_FALSE(ray3d(vec3d::zero(), vec3d::pos_z()) != ray3d(vec3d::zero(), vec3d::pos_z()))
        CER_ASSERT_TRUE(ray3d(vec3d(0, 0, 0), vec3d(0, 0, 1)) != ray3d(vec3d(1, 0, 0), vec3d(0, 0, 1)))
    }

    TEST(ray_test, stream_insertion) {
        std::stringstream str;
        str << ray3d(vec3d::zero(), vec3d::pos_z());
        ASSERT_EQ("{ origin: (0 0 0), direction: (0 0 1) }", str.str());
    }
}
