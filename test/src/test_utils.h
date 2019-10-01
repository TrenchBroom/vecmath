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

#ifndef VECMATH_TEST_UTILS_H
#define VECMATH_TEST_UTILS_H

#include <vecmath/forward.h>

#define CE_ASSERT_TRUE(expr) { constexpr auto _r_r = (expr); ASSERT_TRUE(_r_r); }
#define CE_ASSERT_FALSE(expr) { constexpr auto _r_r = (expr); ASSERT_FALSE(_r_r); }
#define CE_ASSERT_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_EQ((exp), _r_r); }
#define CE_ASSERT_NE(exp, expr) { constexpr auto _r_r = (expr); ASSERT_NE((exp), _r_r); }
#define CE_ASSERT_FLOAT_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_FLOAT_EQ((exp), _r_r); }
#define CE_ASSERT_DOUBLE_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_DOUBLE_EQ((exp), _r_r); }
#define CE_ASSERT_VEC_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_VEC_EQ((exp), _r_r); }
#define CE_ASSERT_MAT_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_MAT_EQ((exp), _r_r); }

#define CER_ASSERT_TRUE(expr) ASSERT_TRUE((expr)); CE_ASSERT_TRUE((expr))
#define CER_ASSERT_FALSE(expr) ASSERT_FALSE((expr)); CE_ASSERT_FALSE((expr))
#define CER_ASSERT_EQ(exp, expr) ASSERT_EQ((exp), (expr)); CE_ASSERT_EQ((exp), (expr))
#define CER_ASSERT_NE(exp, expr) ASSERT_NE((exp), (expr)); CE_ASSERT_NE((exp), (expr))
#define CER_ASSERT_FLOAT_EQ(exp, expr) ASSERT_FLOAT_EQ((exp), (expr)); CE_ASSERT_FLOAT_EQ((exp), (expr))
#define CER_ASSERT_DOUBLE_EQ(exp, expr) ASSERT_DOUBLE_EQ((exp), (expr)); CE_ASSERT_DOUBLE_EQ((exp), (expr))
#define CER_ASSERT_VEC_EQ(exp, expr) ASSERT_VEC_EQ((exp), (expr)); CE_ASSERT_VEC_EQ((exp), (expr))
#define CER_ASSERT_MAT_EQ(exp, expr) ASSERT_MAT_EQ((exp), (expr)); CE_ASSERT_MAT_EQ((exp), (expr))

template <typename T, size_t S>
void ASSERT_VEC_EQ(const vm::vec<T,S>& lhs, const vm::vec<T,S>& rhs) {
    ASSERT_TRUE(is_equal(lhs, rhs, static_cast<T>(0.001)));
}

template <typename T, size_t S>
void EXPECT_VEC_EQ(const vm::vec<T,S>& lhs, const vm::vec<T,S>& rhs) {
    EXPECT_TRUE(is_equal(lhs, rhs, static_cast<T>(0.001)));
}

template <typename T, size_t S>
void ASSERT_VEC_NE(const vm::vec<T,S>& lhs, const vm::vec<T,S>& rhs) {
    ASSERT_FALSE(is_equal(lhs, rhs, static_cast<T>(0.001)));
}

template <typename T, size_t C, size_t R>
void ASSERT_MAT_EQ(const vm::mat<T,R,C>& lhs, const vm::mat<T,R,C>& rhs) {
    ASSERT_TRUE(is_equal(lhs, rhs, static_cast<T>(0.001)));
}

template <typename T, size_t C, size_t R>
void ASSERT_MAT_NE(const vm::mat<T,R,C>& lhs, const vm::mat<T,R,C>& rhs) {
    ASSERT_FALSE(is_equal(lhs, rhs, static_cast<T>(0.001)));
}

#endif //VECMATH_TEST_UTILS_H
