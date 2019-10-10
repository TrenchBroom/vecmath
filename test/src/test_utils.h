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

#ifndef VECMATH_TEST_UTILS_H
#define VECMATH_TEST_UTILS_H

#include <vecmath/forward.h>

#define CE_ASSERT_TRUE(expr) { constexpr auto _r_r = (expr); ASSERT_TRUE(_r_r); }
#define CE_ASSERT_FALSE(expr) { constexpr auto _r_r = (expr); ASSERT_FALSE(_r_r); }
#define CE_ASSERT_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_EQ((exp), _r_r); }
#define CE_ASSERT_NE(exp, expr) { constexpr auto _r_r = (expr); ASSERT_NE((exp), _r_r); }
#define CE_ASSERT_FLOAT_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_FLOAT_EQ((exp), _r_r); }
#define CE_ASSERT_DOUBLE_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_DOUBLE_EQ((exp), _r_r); }
#define CE_ASSERT_NEAR(exp, expr, epsilon) { constexpr auto _r_r = (expr); ASSERT_NEAR((exp), _r_r, (epsilon)); }
#define CE_ASSERT_VEC_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_VEC_EQ((exp), _r_r); }
#define CE_ASSERT_MAT_EQ(exp, expr) { constexpr auto _r_r = (expr); ASSERT_MAT_EQ((exp), _r_r); }

#define CER_ASSERT_TRUE(expr) ASSERT_TRUE((expr)); CE_ASSERT_TRUE((expr))
#define CER_ASSERT_FALSE(expr) ASSERT_FALSE((expr)); CE_ASSERT_FALSE((expr))
#define CER_ASSERT_EQ(exp, expr) ASSERT_EQ((exp), (expr)); CE_ASSERT_EQ((exp), (expr))
#define CER_ASSERT_NE(exp, expr) ASSERT_NE((exp), (expr)); CE_ASSERT_NE((exp), (expr))
#define CER_ASSERT_FLOAT_EQ(exp, expr) ASSERT_FLOAT_EQ((exp), (expr)); CE_ASSERT_FLOAT_EQ((exp), (expr))
#define CER_ASSERT_DOUBLE_EQ(exp, expr) ASSERT_DOUBLE_EQ((exp), (expr)); CE_ASSERT_DOUBLE_EQ((exp), (expr))
#define CER_ASSERT_NEAR(exp, expr, epsilon) ASSERT_NEAR((exp), (expr), (epsilon)); CE_ASSERT_NEAR((exp), (expr), (epsilon))
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
