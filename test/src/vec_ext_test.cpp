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
#include <vecmath/vec_ext.h>

#include "test_utils.h"

#include <array>
#include <vector>

namespace vm {
    // ========== operations on ranges of vectors ==========

    TEST(vec_ext_test, operator_plus_vector) {
        const auto in  = std::vector<vec3f>{ vec3f(1, 2, 3), vec3f(2, 3, 4) };
        const auto exp = std::vector<vec3f>{ vec3f(0, 3, 1), vec3f(1, 4, 2) };
        ASSERT_EQ(exp, in + vec3f(-1, +1, -2));
        ASSERT_EQ(exp, vec3f(-1, +1, -2) + in);
    }

    TEST(vec_ext_test, operator_plus_array) {
        constexpr auto in  = std::array<vec3f, 2>{ vec3f(1, 2, 3), vec3f(2, 3, 4) };
        constexpr auto exp = std::array<vec3f, 2>{ vec3f(0, 3, 1), vec3f(1, 4, 2) };
        CER_ASSERT_EQ(exp, in + vec3f(-1, +1, -2))
        CER_ASSERT_EQ(exp, vec3f(-1, +1, -2) + in)
    }

    TEST(vec_ext_test, operator_multiply_vector) {
        const auto in  = std::vector<vec3f>{ vec3f(1, 2, 3), vec3f(2, 3, 4) };
        const auto exp = std::vector<vec3f>{ vec3f(3, 6, 9), vec3f(6, 9, 12) };
        ASSERT_EQ(exp, in * 3.0f);
        ASSERT_EQ(exp, 3.0f * in);
    }

    TEST(vec_ext_test, operator_multiply_array) {
        constexpr auto in  = std::array<vec3f, 2>{ vec3f(1, 2, 3), vec3f(2, 3, 4) };
        constexpr auto exp = std::array<vec3f, 2>{ vec3f(3, 6, 9), vec3f(6, 9, 12) };
        CER_ASSERT_EQ(exp, in * 3.0f)
        CER_ASSERT_EQ(exp, 3.0f * in)
    }

    TEST(vec_ext_test, parse_valid_string) {
        constexpr auto s = "1.0 3 3.5";

        const auto success = can_parse<float, 3>(s);
        ASSERT_TRUE(success);

        const auto result = parse<float, 3>(s);
        ASSERT_EQ(vec3f(1.0f, 3.0f, 3.5f), result);
    }

    TEST(vec_ext_test, parse_short_string) {
        constexpr auto s = "1.0 3";

        const auto success = can_parse<float, 3>(s);
        ASSERT_FALSE(success);

        const auto result = parse<float, 3>(s);
        ASSERT_EQ(vec3f::zero(), result);
    }

    TEST(vec_ext_test, parse_long_string) {
        constexpr auto s = "1.0 3 4 5";

        const auto success = can_parse<float, 3>(s);
        ASSERT_TRUE(success);

        const auto result = parse<float, 3>(s);
        ASSERT_EQ(vec3f(1.0f, 3.0f, 4.0f), result);
    }

    TEST(vec_ext_test, parse_invalid_string) {
        constexpr auto s = "asdf";

        const auto success = can_parse<float, 3>(s);
        ASSERT_FALSE(success);

        const auto result = parse<float, 3>(s);
        ASSERT_EQ(vec3f::zero(), result);
    }

    TEST(vec_ext_test, parse_empty_string) {
        constexpr auto s = "";

        const auto success = can_parse<float, 3>(s);
        ASSERT_FALSE(success);

        const auto result = parse<float, 3>(s);
        ASSERT_EQ(vec3f::zero(), result);
    }

    TEST(vec_ext_test, parse_all) {
        std::vector<vec3f> result;

        parse_all<float, 3>("", std::back_inserter(result));
        ASSERT_TRUE(result.empty());

        result.clear();
        parse_all<float, 3>("1.0 3 3.5 2.0 2.0 2.0", std::back_inserter(result));
        ASSERT_EQ(std::vector<vec3f>({ vec3f(1, 3, 3.5), vec3f(2, 2, 2) }), result);

        result.clear();
        parse_all<float, 3>("(1.0 3 3.5) (2.0 2.0 2.0)", std::back_inserter(result));
        ASSERT_EQ(std::vector<vec3f>({ vec3f(1, 3, 3.5), vec3f(2, 2, 2) }), result);

        result.clear();
        parse_all<float, 3>("(1.0 3 3.5), (2.0 2.0 2.0)", std::back_inserter(result));
        ASSERT_EQ(std::vector<vec3f>({ vec3f(1, 3, 3.5), vec3f(2, 2, 2) }), result);

        result.clear();
        parse_all<float, 3>("(1.0 3 3.5); (2.0 2.0 2.0)", std::back_inserter(result));
        ASSERT_EQ(std::vector<vec3f>({ vec3f(1, 3, 3.5), vec3f(2, 2, 2) }), result);

        result.clear();
        parse_all<float, 3>("1.0 3 3.5, 2.0 2.0 2.0", std::back_inserter(result));
        ASSERT_EQ(std::vector<vec3f>({ vec3f(1, 3, 3.5), vec3f(2, 2, 2) }), result);
    }
}
