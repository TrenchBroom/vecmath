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
}
