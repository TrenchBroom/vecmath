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

#include <vecmath/constants.h>
#include <vecmath/forward.h>
#include <vecmath/vec.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/segment.h>
#include <vecmath/scalar.h>

#include "test_utils.h"

#include <iterator>
#include <vector>

namespace vm {
    TEST(segment_test, constructor_default) {
        constexpr auto s = segment3d();
        CER_ASSERT_EQ(vec3d::zero(), s.start())
        CER_ASSERT_EQ(vec3d::zero(), s.end())
    }

    TEST(segment_test, constructor_convert) {
        constexpr auto start = vec3d(2, 0, 0);
        constexpr auto end = vec3d(3, 0, 0);
        constexpr auto s = segment3d(start, end);
        constexpr auto t = segment3f(s);
        ASSERT_VEC_EQ(vec3f(start), t.start());
        ASSERT_VEC_EQ(vec3f(end),   t.end());
    }

    TEST(segment_test, constructor_with_points) {
        constexpr auto start = vec3d(3, 0, 0);
        constexpr auto end = vec3d(2, 0, 0);
        constexpr auto s = segment3d(start, end);
        CER_ASSERT_EQ(end, s.start())
        CER_ASSERT_EQ(start, s.end())
    }

    TEST(segment_test, get_origin) {
        constexpr auto s = segment3d(vec3d(3, 0, 0), vec3d(2, 0, 0));
        CER_ASSERT_EQ(s.start(), s.get_origin())
    }

    TEST(segment_test, get_direction) {
        const auto start = vec3d(3, 0, 0);
        const auto end = vec3d(2, 0, 0);
        const auto s = segment3d(start, end);
        ASSERT_EQ(normalize(s.end() - s.start()), s.get_direction());
    }

    TEST(segment_test, length) {
        const auto s = segment3d(vec3d(4, 0, 0), vec3d(2, 0, 0));
        ASSERT_DOUBLE_EQ(2.0, s.length());
    }

    TEST(segment_test, length_c) {
        constexpr auto s = segment3d(vec3d(4, 0, 0), vec3d(2, 0, 0));
        CER_ASSERT_DOUBLE_EQ(2.0, s.length_c())
    }

    TEST(segment_test, squared_length) {
        constexpr auto s = segment3d(vec3d(4, 0, 0), vec3d(2, 0, 0));
        CER_ASSERT_DOUBLE_EQ(4.0, s.squared_length())
    }

    TEST(segment_test, contains1) {
        constexpr auto z = vec3d::zero();
        constexpr auto o = vec3d(1.0, 0.0, 0.0);
        constexpr auto h = vec3d(0.5, 0.0, 0.0);
        constexpr auto n = vec3d(0.5, 1.0, 0.0);

        ASSERT_TRUE( segment3d(z, o).contains(z, Cd::almost_zero()));
        ASSERT_TRUE( segment3d(z, o).contains(h, Cd::almost_zero()));
        ASSERT_TRUE( segment3d(z, o).contains(o, Cd::almost_zero()));
        ASSERT_FALSE(segment3d(z, o).contains(n, Cd::almost_zero()));
    }

    TEST(segment_test, contains2) {
        const auto z = vec3d(-64.0, -64.0, 0.0);
        const auto o = vec3d(  0.0, +64.0, 0.0);

        ASSERT_TRUE( segment3d(z, o).contains(z, Cd::almost_zero()));
        ASSERT_TRUE( segment3d(z, o).contains(o, Cd::almost_zero()));
    }

    TEST(segment_test, transform) {
        constexpr auto s = segment3d(vec3d(0, 0, 0), vec3d(4, 0, 0));
        constexpr auto sm = scaling_matrix(vec3d(2, 0.5, 3));
        constexpr auto tm = translation_matrix(vec3d::one());

        constexpr auto st = s.transform(sm * tm);
        CER_ASSERT_VEC_EQ(sm * tm * s.start(), st.start())
        CER_ASSERT_VEC_EQ(sm * tm * s.end(), st.end())
    }

    TEST(segment_test, translate) {
        constexpr auto s = segment3d(vec3d(0, 0, 0), vec3d(4, 0, 0));
        constexpr auto st = s.translate(vec3d::one());
        CER_ASSERT_VEC_EQ(s.start() + vec3d::one(), st.start())
        CER_ASSERT_VEC_EQ(s.end() + vec3d::one(), st.end())
    }

    TEST(segment_test, center) {
        constexpr auto s = segment3d(vec3d(0, 0, 0), vec3d(4, 0, 0));
        CER_ASSERT_VEC_EQ(vec3d(2, 0, 0), s.center())
    }

    TEST(segment_test, direction) {
        const auto s = segment3d(vec3d(0, 0, 0), vec3d(4, 0, 0));
        ASSERT_VEC_EQ(vec3d::pos_x(), s.direction());
    }

    TEST(segment_test, get_vertices) {
        const auto l = std::vector<segment3d> {
            segment3d(vec3d(0, 0, 0), vec3d(4, 0, 0)),
            segment3d(vec3d(2, 0, 0), vec3d(6, 0, 0))
        };

        auto v = std::vector<vec3d>();
        segment3d::get_vertices(std::begin(l), std::end(l), std::back_inserter(v));

        const auto e = std::vector<vec3d> {
            vec3d(0, 0, 0),
            vec3d(4, 0, 0),
            vec3d(2, 0, 0),
            vec3d(6, 0, 0)
        };

        ASSERT_EQ(e, v);
    }

    TEST(segment_test, compare) {
        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3))
            ) == 0
        )

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(1, 0, 0), vec3d(1, 2, 3))
            ) < 0
        )

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3))
            ) < 0
        )

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(1, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3))
            ) > 0
        )

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3))
            ) > 0
        )

        // with large epsilon
        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                2.0) == 0
        );

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(1, 0, 0), vec3d(1, 2, 3)),
                2.0) == 0
        );

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)),
                2.0) == 0
        );

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(1, 0, 0), vec3d(1, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)),
                2.0) == 0
        );

        CER_ASSERT_TRUE(
            compare(
                segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)),
                segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3))
            ) > 0
        )
    }

    TEST(segment_test, is_equal) {
        CER_ASSERT_TRUE (is_equal(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)), segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)), 0.0))
        CER_ASSERT_FALSE(is_equal(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)), segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)), 0.0))
        CER_ASSERT_TRUE (is_equal(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)), segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)), 2.0))
    }

    TEST(segment_test, operator_equal) {
        CER_ASSERT_TRUE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) == segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) == segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)))
    }

    TEST(segment_test, operator_not_equal) {
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) != segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) != segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)))
    }

    TEST(segment_test, operator_less_than) {
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) < segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)) < segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)) < segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(3, 2, 3)) < segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) < segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)))
    }

    TEST(segment_test, operator_less_than_or_equal) {
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) <= segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)) <= segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)) <= segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(3, 2, 3)) <= segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) <= segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)))
    }

    TEST(segment_test, operator_greater_than) {
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) > segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)) > segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)) > segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(3, 2, 3)) > segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) > segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)))
    }

    TEST(segment_test, operator_greater_than_or_equal) {
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) >= segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)) >= segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_TRUE (segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)) >= segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(3, 2, 3)) >= segment3d(vec3d(2, 0, 0), vec3d(1, 2, 3)))
        CER_ASSERT_FALSE(segment3d(vec3d(0, 0, 0), vec3d(1, 2, 3)) >= segment3d(vec3d(0, 0, 0), vec3d(2, 2, 3)))
    }
}
