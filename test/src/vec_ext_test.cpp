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
#include <vecmath/vec_ext.h>

#include "test_utils.h"

#include <array>
#include <string>
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
