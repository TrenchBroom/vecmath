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

#include "test_utils.h"

#include <vecmath/forward.h>
#include <vecmath/line.h>
#include <vecmath/line_io.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/scalar.h>

#include <sstream>

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

    TEST(line_test, stream_insertion) {
        std::stringstream str;
        str << line3d(line3d(vec3d::zero(), vec3d::pos_z()));
        ASSERT_EQ("{ point: (0 0 0), direction: (0 0 1) }", str.str());
    }
}
