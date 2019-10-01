/*
 Copyright (C) 2010-2017 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VECMATH_MAT_H
#define VECMATH_MAT_H

#include "vec.h"
// #include "quat.h"
#include "constants.h"
#include "util.h"

#include <array>
#include <cassert>
#include <tuple>

namespace vm {
    template <typename T, std::size_t R, std::size_t C>
    class mat {
    public:
        using type = T;
        using column_type = vec<T,R>;
        static const std::size_t rows = R;
        static const std::size_t cols = C;

        /**
         * The matrix components in column major format.
         */
        std::array<column_type, C> v;
    public:
        /* ========== constructors and assignment operators ========== */

        /**
         * Creates a new matrix with its values initialized to an identity matrix.
         */
        constexpr mat() : v{} {
            for (std::size_t c = 0; c < C; c++) {
                for (std::size_t r = 0; r < R; r++) {
                    v[c][r] = c == r ? static_cast<T>(1.0) : static_cast<T>(0.0);
                }
            }
        }

        // Copy and move constructors
        mat(const mat<T, R, C>& other) = default;
        mat(mat<T, R, C>&& other) noexcept = default;

        // Assignment operators
        mat<T, R, C>& operator=(const mat<T, R, C>& other) = default;
        mat<T, R, C>& operator=(mat<T, R, C>&& other) noexcept = default;

        /**
         * Creates a matrix with the given values.
         *
         * @param values the initializer list with the matrix elements in row-major order
         */
        constexpr mat(std::initializer_list<T> values) :
        v{ to_vec_array<T, R, C>(values) } {}

        /**
         * Creates a matrix with the given values.
         *
         * @tparam A11 the type of the first argument
         * @tparam Args the types of the remaining arguments
         * @param a11 the value at position 0,0
         * @param args the remaining values in row-major order
         */
        template <typename A11, typename... Args>
        constexpr explicit mat(const A11 a11, const Args... args) :
        v{ to_vec_array<T, R, C>({ static_cast<T>(a11), static_cast<T>(args)... }) } {}

        /**
         * Creates a matrix with the elements initialized to the values of the corresponding elements of the given
         * matrix. If the given matrix has a different element type, its elements are converted using static_cast.
         *
         * @tparam U the element type of the given matrix
         * @param other the matrix to copy
         */
        template <typename U>
        constexpr explicit mat(const mat<U, R, C>& other) :
        v { cast_matrix_rows<T>(other.v) } {}
    public:
        /* ========== accessors ========== */

        /**
         * Returns the column at the given index.
         *
         * @param index the index of the column to return
         * @return the column at the given index
         */
        constexpr vec<T,R>& operator[] (const std::size_t index) {
            assert(index < C);
            return v[index];
        }

        /**
         * Returns the column at the given index.
         *
         * @param index the index of the column to return
         * @return the column at the given index
         */
        constexpr const vec<T,R>& operator[] (const std::size_t index) const {
            assert(index < C);
            return v[index];
        }
    public:
        /* ========== factory methods ========== */
        /**
         * Returns a matrix where all elements are set to the given value.
         *
         * @param value the value to set
         * @return the newly created matrix
         */
        static constexpr mat<T, R, C> fill(const T value) {
            mat<T, R, C> result;
            for (std::size_t c = 0; c < C; c++) {
                for (std::size_t r = 0; r < R; r++) {
                    result[c][r] = value;
                }
            }
            return result;
        }
    public:
        /**
         * Returns the identity matrix.
         */
        static constexpr mat<T, R, C> identity() {
            constexpr auto result = mat<T, R, C>();
            return result;
        }

        /**
         * Returns a matrix with all elements set to 0.
         */
        static constexpr mat<T, R, C> zero() {
            constexpr auto result = mat<T, R, C>::fill(static_cast<T>(0));
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 90 degrees about the X axis in clockwise direction.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_90_x_cw(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +1, +0, +0, +0,
                +0, +0, +1, +0,
                +0, -1, +0, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 90 degrees about the Y axis in clockwise direction.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_90_y_cw(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +0, +0, -1, +0,
                +0, +1, +0, +0,
                +1, +0, +0, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 90 degrees about the Z axis in clockwise direction.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_90_z_cw(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +0, +1, +0, +0,
                -1, +0, +0, +0,
                +0, +0, +1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 90 degrees about the X axis in counter-clockwise direction.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_90_x_ccw(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +1, +0, +0, +0,
                +0, +0, -1, +0,
                +0, +1, +0, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 90 degrees about the Y axis in counter-clockwise direction.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_90_y_ccw(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +0, +0, +1, +0,
                +0, +1, +0, +0,
                -1, +0, +0, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 90 degrees about the Z axis in counter-clockwise direction.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_90_z_ccw(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +0, -1, +0, +0,
                +1, +0, +0, +0,
                +0, +0, +1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 180 degrees about the X axis.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_180_x(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +1, +0, +0, +0,
                +0, -1, +0, +0,
                +0, +0, -1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 180 degrees about the Y axis.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_180_y(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                -1, +0, +0, +0,
                +0, +1, +0, +0,
                +0, +0, -1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that rotates by 180 degrees about the Z axis.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> rot_180_z(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                -1, +0, +0, +0,
                +0, -1, +0, +0,
                +0, +0, +1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that mirrors along the X axis.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> mirror_x(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                -1, +0, +0, +0,
                +0, +1, +0, +0,
                +0, +0, +1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that mirrors along the Y axis.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> mirror_y(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +1, +0, +0, +0,
                +0, -1, +0, +0,
                +0, +0, +1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a 4x4 matrix that mirrors along the Z axis.
         */
        template <std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> mirror_z(typename std::enable_if<RR == 4 && CC == 4>::type* = nullptr) {
            constexpr auto result = mat<T, R, C>(
                +1, +0, +0, +0,
                +0, +1, +0, +0,
                +0, +0, -1, +0,
                +0, +0, +0, +1);
            return result;
        }

        /**
         * Returns a matrix that sets the given component of a vector to 0 if multiplied by it.
         *
         * @tparam E the index of the component to set to zero
         * @return the matrix
         */
        template <std::size_t E, std::size_t RR = R, std::size_t CC = C>
        static constexpr mat<T, R, C> zero_out(typename std::enable_if<RR == CC>::type* = nullptr) {
            static_assert(E < R, "Element index out of bounds");
            constexpr auto result = set(mat<T, R, C>::identity(), E, E, static_cast<T>(0));
            return result;
        }
    };

    /* ========== comparison operators ========== */

    /**
     * Compares the given two matrices column wise.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @param epsilon the epsilon value
     * @return a negative value if there is a column in the left matrix that compares less than its corresponding
     * column of the right matrix, a positive value in the opposite case, and 0 if all columns compare equal
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr int compare(const mat<T, R, C>& lhs, const mat<T, R, C>& rhs, const T epsilon = static_cast<T>(0.0)) {
        for (std::size_t c = 0; c < C; c++) {
            const auto cmp = compare(lhs[c], rhs[c], epsilon);
            if (cmp != 0) {
                return cmp;
            }
        }
        return 0;
    }

    /**
     * Checks whether the given matrices have equal components.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @param epsilon the epsilon value
     * @return true if all components of the given matrices are equal, and false otherwise
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr bool is_equal(const mat<T, R, C>& lhs, const mat<T, R, C>& rhs, const T epsilon) {
        return compare(lhs, rhs, epsilon) == 0;
    }

    /**
     * Checks whether all columns of the given matrix are zero.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param m the matrix to check
     * @param epsilon the epsilon value
     * @return true if all columsn of the given matrix are zero
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr bool is_zero(const mat<T,R,C>& m, const T epsilon) {
        for (size_t c = 0; c < C; ++c) {
            if (!is_zero(m[c], epsilon)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Checks whether the given matrices have identical components.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @return true if all components of the given matrices are equal, and false otherwise
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr bool operator==(const mat<T, R, C>& lhs, const mat<T, R, C>& rhs) {
        return compare(lhs, rhs, static_cast<T>(0.0)) == 0;
    }

    /**
     * Checks whether the given matrices have identical components.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @return false if all components of the given matrices are equal, and true otherwise
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr bool operator!=(const mat<T, R, C>& lhs, const mat<T, R, C>& rhs) {
        return compare(lhs, rhs, static_cast<T>(0.0)) != 0;
    }

    /* ========== arithmetic operators ========== */

    /**
     * Returns a copy of the given matrix.
     *
     * @param m the matrix
     * @return a copy of the given matrix
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator+(const mat<T, R, C>& m) {
        return m;
    }

    /**
     * Returns a matrix with the negated components of the given matrix.
     *
     * @param m the matrix to negate
     * @return the negated matrix
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator-(const mat<T, R, C>& m) {
        mat<T, R, C> result;
        for (size_t c = 0; c < C; c++) {
            result[c] = -m[c];
        }
        return result;
    }

    /**
     * Computes the sum of two matrices by adding the corresponding components.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @return a matrix where each component is the sum of the two corresponding components of the given matrices
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator+(const mat<T, R, C>& lhs, const mat<T, R, C>& rhs) {
        mat<T, R, C> result;
        for (size_t c = 0; c < C; c++) {
            result[c] = lhs[c] + rhs[c];
        }
        return result;
    }

    /**
     * Computes the difference of two matrices by subtracting the corresponding components.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @return a matrix where each component is the difference of the two corresponding components of the given matrices
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator-(const mat<T, R, C>& lhs, const mat<T, R, C>& rhs) {
        mat<T, R, C> result;
        for (size_t c = 0; c < C; c++) {
            result[c] = lhs[c] - rhs[c];
        }
        return result;
    }

    /**
     * Computes the product of the given two matrices.
     *
     * @tparam T the component type
     * @tparam R1 the number of rows of the first matrix
     * @tparam C1R2 the number of columns of the first matrix and the number of rows of the second matrix
     * @tparam C2 the number of columns of the second matrix
     * @param lhs the first matrix
     * @param rhs the second matrix
     * @return the product of the given matrices
     */
    template <typename T, std::size_t R1, std::size_t C1R2, std::size_t C2>
    constexpr mat<T, R1, C2> operator*(const mat<T, R1, C1R2>& lhs, const mat<T, C1R2, C2>& rhs) {
        auto result = mat<T, R1, C2>::zero();
        for (size_t c = 0; c < C2; c++) {
            for (size_t r = 0; r < R1; r++) {
                for (size_t i = 0; i < C1R2; ++i) {
                    result[c][r] += lhs[i][r] * rhs[c][i];
                }
            }
        }
        return result;
    }

    /**
     * Computes the scalar product of the given matrix and the given value.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the matrix
     * @param rhs the scalar
     * @return the product of the given matrix and the given value
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator*(const mat<T, R, C>& lhs, const T rhs) {
        mat<T, R, C> result;
        for (size_t c = 0; c < C; ++c) {
            result[c] = lhs[c] * rhs;
        }
        return result;
    }

    /**
     * Computes the scalar product of the given matrix and the given value.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the scalar
     * @param rhs the matrix
     * @return the product of the given matrix and the given value
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator*(const T lhs, const mat<T, R, C>& rhs) {
        return rhs * lhs;
    }

    /**
     * Computes the scalar division of the given matrix and the given value.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the matrix
     * @param rhs the scalar
     * @return the division of the given matrix and the given value
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> operator/(const mat<T, R, C>& lhs, const T rhs) {
        mat<T, R, C> result;
        for (size_t c = 0; c < C; ++c) {
            result[c] = lhs[c] / rhs;
        }
        return result;
    }

    /**
     * Multiplies the given vector by the given matrix.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the vector
     * @param rhs the matrix
     * @return the product of the given vector and the given matrix
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr vec<T,R> operator*(const vec<T,R>& lhs, const mat<T, R, C>& rhs) {
        vec<T,R> result;
        for (size_t c = 0; c < C; c++) {
            result[c] = dot(lhs, rhs[c]);
        }
        return result;
    }

    /**
     * Multiplies the given vector by the given matrix.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the matrix
     * @param rhs the vector
     * @return the product of the given vector and the given matrix
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr vec<T,R> operator*(const mat<T, R, C>& lhs, const vec<T,C>& rhs) {
        vec<T,C> result;
        for (size_t r = 0; r < R; r++) {
            for (size_t c = 0; c < C; ++c) {
                result[r] += lhs[c][r] * rhs[c];
            }
        }
        return result;
    }

    /**
     * Multiplies the given vector by the given matrix.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the matrix
     * @param rhs the vector
     * @return the product of the given vector and the given matrix
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr vec<T,C-1> operator*(const mat<T, R, C>& lhs, const vec<T,C-1>& rhs) {
        return toCartesianCoords(lhs * toHomogeneousCoords(rhs));
    }

    /**
     * Multiplies the given vector by the given matrix.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param lhs the vector
     * @param rhs the matrix
     * @return the product of the given vector and the given matrix
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr vec<T,R-1> operator*(const vec<T,R-1>& lhs, const mat<T, R, C>& rhs) {
        return toCartesianCoords(toHomogeneousCoords(lhs) * rhs);
    }

    /**
     * Returns a matrix with the given element set to the given value.
     *
     * @tparam T the element type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param m the matrix
     * @param r the row index
     * @param c the column index
     * @param v the value
     *
     * @return m with the element set at the given row and column to the given value
     */
    template <typename T, std::size_t R, std::size_t C>
    constexpr mat<T, R, C> set(mat<T, R, C> m, const std::size_t r, const std::size_t c, const T v) {
        m[c][r] = v;
        return m;
    }

    /**
     * Transposes the given square matrix.
     *
     * @tparam T the component type
     * @tparam S the number of rows and columns
     * @param m the matrix to transpose
     * @return the transposed matrix
     */
    template <typename T, std::size_t S>
    constexpr mat<T,S,S> transpose(const mat<T,S,S>& m) {
        using std::swap;
        mat<T,S,S> result(m);
        for (size_t c = 0; c < S; c++) {
            for (size_t r = c + 1; r < S; r++) {
                swap(result[c][r], result[r][c]);
            }
        }
        return result;
    }
}

#endif
